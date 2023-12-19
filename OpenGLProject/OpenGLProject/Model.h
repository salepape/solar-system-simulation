#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

class Mesh;
class Renderer;
class Texture;



class Model
{
public:
	Model(const std::string& path, const bool gammaCorrectionArg = false);
	~Model();

	inline std::vector<Texture> GetTextures() const { return loadedTextures; }
	inline std::vector<Mesh> GetMeshes() const { return meshes; }

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