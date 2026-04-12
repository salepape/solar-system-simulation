#include "Model.h"

#include <algorithm>
#include <assimp/defs.h>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <cassert>
#include <glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <utility>

#include "OpenGLProject/Buffers/VertexBuffer.h"
#include "OpenGLProject/Rendering/Renderer.h"
#include "OpenGLProject/Utils/Utils.h"



Model::Model(const std::filesystem::path& inPath, const ShaderLookUpID::Enum inShaderLookUpID, const bool inGammaCorrection) :
	shaderLookUpID(inShaderLookUpID), gammaCorrection(inGammaCorrection)
{
	LoadModel(inPath);
}

void Model::LoadModel(const std::filesystem::path& path)
{
	// Open-source model importer library allowing to convert various 3D Model formats (.obj with .mtl companion for us, .gltf, etc.) into a uniform one
	Assimp::Importer importer;

	const aiScene* const scene = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		std::cout << "ERROR::ASSIMP - Error when reading model file located at " << path.string() << ": " << importer.GetErrorString() << std::endl;
		assert(false);
	}

	ProcessMeshNode(*scene->mRootNode, *scene);
}

void Model::ProcessMeshNode(const aiNode& node, const aiScene& scene)
{
	meshes.reserve(node.mNumMeshes);
	materials.reserve(node.mNumMeshes);

	for (uint32_t i = 0; i < node.mNumMeshes; ++i)
	{
		// Get mesh from the scene (node object's purpose is to index scene objects)
		const aiMesh* const meshPtr = scene.mMeshes[node.mMeshes[i]];
		if (meshPtr == nullptr)
		{
			std::cout << "ERROR::ASSIMP - No mesh found in array mMeshes index " << node.mMeshes[i] << std::endl;
			assert(false);
		}

		// A single Mesh instance and a single Material instance will be created once both methods below have been called
		const aiMesh& mesh = *meshPtr;
		ProcessMesh(mesh);
		ProcessMaterial(mesh, scene);
	}

	// Process ASSIMP children nodes recursively
	for (uint32_t i = 0; i < node.mNumChildren; ++i)
	{
		ProcessMeshNode(*node.mChildren[i], scene);
	}
}

void Model::ProcessMesh(const aiMesh& mesh)
{
	meshes.emplace_back(MeshComponent(ProcessMeshVertices(mesh), ProcessMeshIndices(mesh)));
}

std::vector<Vertex> Model::ProcessMeshVertices(const aiMesh& mesh)
{
	std::vector<Vertex> vertices;
	vertices.reserve(mesh.mNumVertices);

	for (uint32_t i = 0; i < mesh.mNumVertices; ++i)
	{
		Vertex vertex;

		vertex.position = glm::vec3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z);
		vertex.normal = glm::vec3(mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z);

		if (mesh.mTextureCoords[0])
		{
			// Only supporting models with a single set of texture coordinates (out of 8 theoretically) for now
			vertex.texCoords = glm::vec2(mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y);
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f);
		}

		vertex.tangent = glm::vec3(mesh.mTangents[i].x, mesh.mTangents[i].y, mesh.mTangents[i].z);
		vertex.biTangent = glm::vec3(mesh.mBitangents[i].x, mesh.mBitangents[i].y, mesh.mBitangents[i].z);

		vertices.push_back(std::move(vertex));
	}

	return vertices;
}

std::vector<uint32_t> Model::ProcessMeshIndices(const aiMesh& mesh)
{
	std::vector<uint32_t> indices;
	indices.reserve(mesh.mNumFaces);

	for (uint32_t i = 0; i < mesh.mNumFaces; ++i)
	{
		const aiFace& facet = mesh.mFaces[i];

		for (uint32_t j = 0; j < facet.mNumIndices; ++j)
		{
			indices.push_back(facet.mIndices[j]);
		}
	}

	return indices;
}

void Model::ProcessMaterial(const aiMesh& mesh, const aiScene& scene)
{
	const aiMaterial* const material = scene.mMaterials[mesh.mMaterialIndex];
	if (material == nullptr)
	{
		std::cout << "ERROR::ASSIMP - No materials found in array mMaterials index " << mesh.mMaterialIndex << std::endl;
		assert(false);
	}

	// Material .mtl file has not been provided with the Model, so it needs to be created from code using GLSL Vertex/Fragment Shaders
	if (material->mNumProperties == 0)
	{
		materials.emplace_back(ShaderLookUpID::Enum::UNDEFINED, ProcessTextures(*material));
	}
	// Material Model is using is provided in a .mtl file, so just process data out of it
	else
	{
		// Read 'Kd' factor in .mtl file
		aiColor3D diffuseColour;
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColour);

		const DiffuseProperties& diffuseProperties = { glm::vec3(diffuseColour.r, diffuseColour.g, diffuseColour.b) };

		// Read 'Ks' factor in .mtl file
		aiColor3D specularColour;
		material->Get(AI_MATKEY_COLOR_SPECULAR, specularColour);

		// Read 'Ns' factor in .mtl file
		ai_real shininess;
		material->Get(AI_MATKEY_SHININESS, shininess);

		const SpecularProperties& specularProperties = { glm::vec3(specularColour.r, specularColour.g, specularColour.b), shininess };

		// Read 'd' factor in .mtl file
		ai_real transparency;
		material->Get(AI_MATKEY_COLOR_TRANSPARENT, transparency);

		// @todo - Read 'illum' factor in .mtl file, corresponding to 'IsBlinn' parameter?

		materials.emplace_back(shaderLookUpID, ProcessTextures(*material), diffuseProperties, specularProperties, transparency);
	}
}

std::vector<Texture> Model::ProcessTextures(const aiMaterial& material)
{
	// Best we can do is to assume we have a single Texture per Mesh, and reserve memory as such
	std::vector<Texture> textures;

	// Needed due to how ASSIMP implemented GetTextureCount() below
	for (const TextureType::Enum& textureType : TextureType::All)
	{
		const aiTextureType& assimpTextureType = static_cast<aiTextureType>(textureType);

		const uint32_t textureCount = material.GetTextureCount(assimpTextureType);
		for (uint32_t i = 0; i < textureCount; ++i)
		{
			// Get texture path from Material .mtl file (remove any comments at its right)
			aiString texturePathMtlLine;
			material.GetTexture(assimpTextureType, i, &texturePathMtlLine);
			const std::string texturePathMtlString(texturePathMtlLine.C_Str());
			const std::string textureImagePath(FileUtils::GetTexturePathFromMtlLine(texturePathMtlString));

			// Skip texture creation if already done
			const auto& loadedTextureIt = find_if(materials.begin(), materials.end(), [&textureImagePath](const Material& inLoadedMaterial)
			{
				for (const Texture& texture : inLoadedMaterial.GetTextures())
				{
					if (texture.GetImagePath() == textureImagePath)
					{
						return true;
					}
				}

				return false;
			});

			if (loadedTextureIt != materials.end())
			{
				continue;
			}

			// Create a DDS texture from the ASSIMP one
			Texture texture(textureImagePath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, textureType);
			texture.LoadDDS();
			textures.push_back(std::move(texture));
		}
	}

	return textures;
}

void Model::StoreInstanceModelMatrices(const std::vector<glm::mat4>& modelMatrices, const std::size_t sizeInBytes) const
{
	// Configure instanced array
	VertexBuffer vbo(static_cast<const void*>(modelMatrices.data()), sizeInBytes);

	// Set transformation matrices as an instance vertex attribute for each mesh VAO already created
	for (const MeshComponent& mesh : meshes)
	{
		mesh.StoreInstanceModelMatrices(vbo);
	}

	vbo.Unbind();
}

void Model::Render(const Renderer& renderer) const
{
	for (const MeshComponent& mesh : meshes)
	{
		mesh.Render(renderer);
	}
}

void Model::RenderInstances(const Renderer& renderer, const uint32_t instanceCount) const
{
	for (const MeshComponent& mesh : meshes)
	{
		mesh.RenderInstances(renderer, instanceCount);
	}
}