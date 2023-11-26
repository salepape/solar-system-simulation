#include "Model.h"



Model::Model(const std::string& path, const bool gammaCorrectionArg) : gammaCorrection(gammaCorrectionArg)
{
	LoadModel(path);
}

Model::~Model()
{

}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		std::cout << "ERROR::ASSIMP - Error when reading file: " << importer.GetErrorString() << std::endl;
		return;
	}

	// Process ASSIMP's root node recursively
	ProcessNode(*scene->mRootNode, *scene);
}

void Model::ProcessNode(const aiNode& node, const aiScene& scene)
{
	// Process each mesh located at the current node
	for (unsigned int i = 0; i < node.mNumMeshes; ++i)
	{
		// Node object only contains indices to index the actual objects in the scene
		// Scene contains all the data, node is just to keep stuff organised (like relations between nodes)
		const aiMesh* mesh = scene.mMeshes[node.mMeshes[i]];
		meshes.push_back(ProcessMesh(*mesh, scene));
	}

	// Process ASSIMP's children nodes recursively
	for (unsigned int i = 0; i < node.mNumChildren; ++i)
	{
		ProcessNode(*node.mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(const aiMesh& mesh, const aiScene& scene)
{
	// ASSIMP doesn't use glm::vec3 class, so we store all the vertex information into one
	std::vector<Vertex> vertices;
	for (unsigned int i = 0; i < mesh.mNumVertices; ++i)
	{
		Vertex vertex;

		vertex.Position = glm::vec3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z);
		vertex.Normal = glm::vec3(mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z);

		if (mesh.mTextureCoords[0])
		{
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
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

	// Store the indices of each mesh facet
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh.mNumFaces; ++i)
	{
		const aiFace facet = mesh.mFaces[i];

		for (unsigned int j = 0; j < facet.mNumIndices; ++j)
		{
			indices.push_back(facet.mIndices[j]);
		}
	}

	// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to specular and normal textures.
	std::vector<Texture> textures;
	const aiMaterial* material = scene.mMaterials[mesh.mMaterialIndex];

	const std::vector<Texture> diffuseMaps = LoadTextures(*material, aiTextureType_DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	const std::vector<Texture> specularMaps = LoadTextures(*material, aiTextureType_SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	const std::vector<Texture> normalMaps = LoadTextures(*material, aiTextureType_NORMALS);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	const std::vector<Texture> heightMaps = LoadTextures(*material, aiTextureType_HEIGHT);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadTextures(const aiMaterial& material, const aiTextureType type)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < material.GetTextureCount(type); ++i)
	{
		aiString TexturePath;
		material.GetTexture(type, i, &TexturePath);

		// Skip all textures with the same file path since it means it has already been loaded
		bool textureAlreadyLoaded = false;
		for (unsigned int j = 0; j < loadedTextures.size(); ++j)
		{
			if (std::strcmp(loadedTextures[j].GetPath(), TexturePath.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				textureAlreadyLoaded = true;
				break;
			}
		}

		if (textureAlreadyLoaded == false)
		{
			// @todo - Implement correspondance aiTextureType and MapType enums
			Texture texture(TexturePath.C_Str(), GL_TEXTURE_2D, GeometryType::MODEL, MapType::NONE);
			texture.LoadDDS();
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
	}

	return textures;
}

void Model::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Render(renderer, textureUnit);
	}
}



