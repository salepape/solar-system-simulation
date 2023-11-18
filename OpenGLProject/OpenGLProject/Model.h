#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <map>
#include <SOIL2.h>

#include "mesh.h"	



class Model
{
private:
	// Stores all the textures loaded so far, to make sure each is not loaded more than once
	std::vector<Texture> loadedTextures;
	std::vector<Mesh> meshes;
	bool gammaCorrection;

	// Load a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector
	void LoadModel(const std::string& path);

	// Process a node in a recursive fashion
	void ProcessNode(const aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);

	// Check all material textures of a given type and loads the textures if they're not loaded yet
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	Model(const std::string& path, const bool gammaCorrectionArg = false);
	~Model();

	void Render(const Renderer& renderer, const unsigned int& textureUnit);

	inline std::vector<Texture> GetTextures() const { return loadedTextures; }
	inline std::vector<Mesh> GetMeshes() const { return meshes; }
};



#endif // MODEL_H