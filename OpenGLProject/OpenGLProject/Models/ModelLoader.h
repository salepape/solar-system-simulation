#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <cstdint>
#include <filesystem>
#include <vector>

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;
class Model;
class Texture;
struct Vertex;



// Stateless ASSIMP loader that reads Mesh/Material info from .obj/.mtl files, and add instances to the relevant vectors owned by the Model passed in argument
class ModelLoader
{
public:
	// Load a model from its 3D format using ASSIMP model importer (.obj verified, .gltf not tested) and stores the resulting data in Mesh/Material vectors
	static void LoadModel(Model& model, const std::filesystem::path& path);

private:
	// Process an ASSIMP mesh node recursively by transferring mesh data to Vertex-compatible vector
	static void ProcessMeshNode(Model& model, const aiNode& node, const aiScene& scene);

	// Retrieve ASSIMP mesh data (in .obj Material file) and store Mesh instances in the associated vector
	static void ProcessMesh(Model& model, const aiMesh& mesh);

	// Retrieve ASSIMP vertex data (in .obj Material file) and build a vector of Vertex instances out of it
	static std::vector<Vertex> ProcessMeshVertices(const aiMesh& mesh);

	// Retrieve ASSIMP index data (in .obj Material file) and build a vector of (Vertex) indexes out of it
	static std::vector<uint32_t> ProcessMeshIndices(const aiMesh& mesh);

	// Retrieve ASSIMP material data (in .mtl Material companion file) and store Material instances in the associated vector
	static void ProcessMaterial(Model& model, const aiMesh& mesh, const aiScene& scene);

	// Retrieve ASSIMP texture data (in .mtl Material companion file) and build a vector of Texture instances out of it
	static std::vector<Texture> ProcessTextures(Model& model, const aiMaterial& material);
};



#endif // MODEL_LOADER_H