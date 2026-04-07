#include "Model.h"

#include <algorithm>
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

#include "Renderer.h"
#include "Utils.h"
#include "VertexBuffer.h"



Model::Model(const std::filesystem::path& inPath, const bool inGammaCorrection) :
	gammaCorrection(inGammaCorrection)
{
	LoadModel(inPath);
}

void Model::LoadModel(const std::filesystem::path& path)
{
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
	// Best we can do is to assume we have a single Texture per Mesh, and reserve memory as such
	meshes.reserve(node.mNumMeshes);
	textures.reserve(node.mNumMeshes);

	for (uint32_t i = 0; i < node.mNumMeshes; ++i)
	{
		// Get mesh from the scene (node object's purpose is to index scene objects)
		const aiMesh* const meshPtr = scene.mMeshes[node.mMeshes[i]];
		if (meshPtr == nullptr)
		{
			std::cout << "ERROR::ASSIMP - No mesh found in array mMeshes index " << node.mMeshes[i] << std::endl;
			assert(false);
		}

		// A single Mesh instance and 1+ Texture instances will be created once both methods below have been called
		const aiMesh& mesh = *meshPtr;
		ProcessMesh(mesh);
		ProcessTextures(mesh, scene);
	}

	// Process ASSIMP children nodes recursively
	for (uint32_t i = 0; i < node.mNumChildren; ++i)
	{
		ProcessMeshNode(*node.mChildren[i], scene);
	}
}

void Model::ProcessMesh(const aiMesh& mesh)
{
	meshes.emplace_back(Mesh(ProcessMeshVertices(mesh), ProcessMeshIndices(mesh)));
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

void Model::ProcessTextures(const aiMesh& mesh, const aiScene& scene)
{
	const aiMaterial* const material = scene.mMaterials[mesh.mMaterialIndex];
	if (material == nullptr)
	{
		std::cout << "ERROR::ASSIMP - No materials found in array mMaterials index " << mesh.mMaterialIndex << std::endl;
		assert(false);
	}

	// @todo - Create Materials out of aiMaterials instead of Textures?

	for (const TextureType::Enum& textureType : TextureType::All)
	{
		const aiTextureType& assimpTextureType = static_cast<aiTextureType>(textureType);

		const uint32_t textureCount = material->GetTextureCount(assimpTextureType);

		for (uint32_t i = 0; i < textureCount; ++i)
		{
			// Get texture path (remove any comments at its right)
			aiString texturePathMtlLine;
			material->GetTexture(assimpTextureType, i, &texturePathMtlLine);
			const std::string texturePathMtlString(texturePathMtlLine.C_Str());
			const std::string textureImagePath(FileUtils::GetTexturePathFromMtlLine(texturePathMtlString));

			// Skip texture creation if already done
			const auto& loadedTextureIt = find_if(textures.begin(), textures.end(), [&textureImagePath](const Texture& loadedTexture)
			{
				return textureImagePath == loadedTexture.GetImagePath();
			});

			if (loadedTextureIt != textures.end())
			{
				continue;
			}

			// Create a DDS texture from the ASSIMP one
			Texture texture(textureImagePath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, textureType);
			texture.LoadDDS();
			textures.push_back(std::move(texture));
		}
	}
}

void Model::StoreInstanceModelMatrices(const std::vector<glm::mat4>& modelMatrices, const std::size_t sizeInBytes) const
{
	// Configure instanced array
	VertexBuffer vbo(static_cast<const void*>(modelMatrices.data()), sizeInBytes);

	// Set transformation matrices as an instance vertex attribute for each mesh VAO already created
	for (const Mesh& mesh : meshes)
	{
		mesh.StoreInstanceModelMatrices(vbo);
	}

	vbo.Unbind();
}

void Model::Render(const Renderer& renderer) const
{
	for (const Mesh& mesh : meshes)
	{
		mesh.Render(renderer);
	}
}

void Model::RenderInstances(const Renderer& renderer, const uint32_t instanceCount) const
{
	for (const Mesh& mesh : meshes)
	{
		mesh.RenderInstances(renderer, instanceCount);
	}
}