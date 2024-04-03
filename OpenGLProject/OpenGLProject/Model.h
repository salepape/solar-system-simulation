#ifndef MODEL_H
#define MODEL_H

#include <assimp/material.h>
#include <iostream>
#include <string>
#include <vector>

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;
class Mesh;
class Renderer;
class Texture;



class Model
{
public:
	Model(const std::string& inPath, const bool inGammaCorrection = false);
	~Model();

	std::vector<Texture> GetTextures() const { return loadedTextures; }
	std::vector<Mesh> GetMeshes() const { return meshes; }

	void Render(const Renderer& renderer, const unsigned int& textureUnit);

private:
	// Stores all the textures loaded so far to make sure each is only loaded once
	std::vector<Texture> loadedTextures;
	std::vector<Mesh> meshes;
	bool gammaCorrection{ false };

	// Load a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector
	void LoadModel(const std::string& path);

	// Process a node recursively
	void ProcessNode(const aiNode& node, const aiScene& scene);
	Mesh ProcessMesh(const aiMesh& mesh, const aiScene& scene);

	// Check all textures of a given type for a given material and loads them if they're not loaded yet
	std::vector<Texture> LoadTextures(const aiMaterial& material, const aiTextureType type);
};



#endif // MODEL_H