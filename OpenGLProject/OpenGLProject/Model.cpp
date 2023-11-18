#include "Model.h"



Model::Model(const std::string& path, const bool gammaCorrectionArg) : gammaCorrection(gammaCorrectionArg)
{
	LoadModel(path);
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP: " << importer.GetErrorString() << std::endl;
		return;
	}

	// Process ASSIMP's root node recursively
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(const aiNode* node, const aiScene* scene)
{
	// Process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		// Node object only contains indices to index the actual objects in the scene
		// Scene contains all the data, node is just to keep stuff organized (like relations between nodes)
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	// Process ASSIMP's children nodes recursively
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
{
	// Store the vertices of the mesh
	std::vector<Vertex> vertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		// ASSIMP doesn't use the same vec3 class, so we store all the information into a glm::vec3
		glm::vec3 vector;
		
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) 
		{
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;

		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;

		vertices.push_back(vertex);
	}

	// Store the indices of each mesh facet
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace facet = mesh->mFaces[i];

		for (unsigned int j = 0; j < facet.mNumIndices; ++j)
			indices.push_back(facet.mIndices[j]);
	}

	// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to specular and normal textures.
	std::vector<Texture> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// Diffuse maps 
	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());		

	// Specular maps
	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// Normal maps
	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// Height maps
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(const aiMaterial* mat, const aiTextureType type, const std::string typeName)
{
	std::vector<Texture> materialTextures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString TexturePath;
		mat->GetTexture(type, i, &TexturePath);

		// Check if texture was loaded before and if so, continue to next iteration
		bool skip = false;
		for (unsigned int j = 0; j < loadedTextures.size(); ++j)
		{
			// A texture with the same filepath has already been loaded: continue to next one!
			if (std::strcmp(loadedTextures[j].GetPath(), TexturePath.C_Str()) == 0)
			{
				materialTextures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture(TexturePath.C_Str(), typeName.c_str(), GL_TEXTURE_2D, ObjectType::DEFAULT);
			texture.LoadDDS();
			materialTextures.push_back(texture);
			loadedTextures.push_back(texture);
		}
	}

	return materialTextures;
}

Model::~Model()
{

}

void Model::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	for (unsigned int i = 0; i < meshes.size(); ++i)
		meshes[i].Render(renderer, textureUnit);
}



