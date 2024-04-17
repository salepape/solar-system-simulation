#include "Model.h"

#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Mesh.h"
#include "Texture.h"



Model::Model(const std::string& inPath, const bool inGammaCorrection) : gammaCorrection(inGammaCorrection)
{
	LoadModel(inPath);
}

Model::~Model()
{

}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;

	const aiScene* const scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		std::cout << "ERROR::ASSIMP - Error when reading file: " << importer.GetErrorString() << std::endl;
		return;
	}

	// Process ASSIMP root node recursively
	ProcessNode(*scene->mRootNode, *scene);
}

void Model::ProcessNode(const aiNode& node, const aiScene& scene)
{
	// Process each mesh located at the current node
	meshes.reserve(node.mNumMeshes);
	for (unsigned int i = 0; i < node.mNumMeshes; ++i)
	{
		// Get mesh from the scene (that contains data), node object only indexes scene objects
		const aiMesh* const mesh = scene.mMeshes[node.mMeshes[i]];
		if (mesh == nullptr)
		{
			std::cout << "ERROR::ASSIMP - No mesh found in array mMeshes index " << node.mMeshes[i] << std::endl;
			continue;
		}

		meshes.push_back(ProcessMesh(*mesh, scene));
	}

	// Process ASSIMP children nodes recursively
	for (unsigned int i = 0; i < node.mNumChildren; ++i)
	{
		ProcessNode(*node.mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(const aiMesh& mesh, const aiScene& scene)
{
	const std::vector<Vertex>& vertices = GetMeshVertices(mesh);
	const std::vector<unsigned int>& indices = GetMeshIndices(mesh);
	GetMeshTextures(mesh, scene);

	return Mesh(vertices, indices, textures);
}

std::vector<Vertex> Model::GetMeshVertices(const aiMesh& mesh)
{
	std::vector<Vertex> vertices;
	vertices.reserve(mesh.mNumVertices);
	for (unsigned int i = 0; i < mesh.mNumVertices; ++i)
	{
		Vertex vertex;

		vertex.Position = glm::vec3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z);
		vertex.Normal = glm::vec3(mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z);

		if (mesh.mTextureCoords[0])
		{
			// Only supporting models with a single set of texture coordinates (out of 8 theoretically) for now
			vertex.TexCoords = glm::vec2(mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y);
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertex.Tangent = glm::vec3(mesh.mTangents[i].x, mesh.mTangents[i].y, mesh.mTangents[i].z);
		vertex.Bitangent = glm::vec3(mesh.mBitangents[i].x, mesh.mBitangents[i].y, mesh.mBitangents[i].z);

		vertices.push_back(vertex);
	}

	return vertices;
}

std::vector<unsigned int> Model::GetMeshIndices(const aiMesh& mesh)
{
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh.mNumFaces; ++i)
	{
		const aiFace& facet = mesh.mFaces[i];

		for (unsigned int j = 0; j < facet.mNumIndices; ++j)
		{
			indices.push_back(facet.mIndices[j]);
		}
	}

	return indices;
}

void Model::GetMeshTextures(const aiMesh& mesh, const aiScene& scene)
{
	const aiMaterial* const material = scene.mMaterials[mesh.mMaterialIndex];
	if (material == nullptr)
	{
		std::cout << "ERROR::ASSIMP - No materials found in array mMaterials index " << mesh.mMaterialIndex << std::endl;
		return;
	}

	const std::vector<aiTextureType> textureTypes{ aiTextureType_AMBIENT, aiTextureType_DIFFUSE, aiTextureType_SPECULAR };
	for (const auto& type : textureTypes)
	{
		const unsigned int textureCount = material->GetTextureCount(type);
		textures.reserve(textureCount);
		for (unsigned int i = 0; i < textureCount; ++i)
		{
			// Get texture path
			aiString texturePath;
			material->GetTexture(type, i, &texturePath);
			const std::string& textureStringPath = std::string(texturePath.C_Str(), texturePath.length);

			// Skip texture creation if already done
			const auto loadedTextureIt = find_if(textures.begin(), textures.end(), [&textureStringPath](const Texture& loadedTexture)
			{
				return textureStringPath == loadedTexture.GetPath();
			});

			if (loadedTextureIt != textures.end())
			{
				continue;
			}

			// Create a DDS texture from the ASSIMP one
			Texture texture(textureStringPath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, static_cast<TextureType>(type));
			texture.LoadDDS();
			textures.push_back(texture);
		}
	}
}

void Model::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	for (auto& mesh: meshes)
	{
		mesh.Render(renderer, textureUnit);
	}
}