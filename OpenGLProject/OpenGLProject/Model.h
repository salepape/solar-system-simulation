#ifndef MODEL_H
#define MODEL_H

#include <cstddef> // std::size_t
#include <cstdint>
#include <filesystem>
#include <glm/mat4x4.hpp>
#include <vector>

#include "Mesh.h"
#include "Texture.h"

struct aiMesh;
struct aiNode;
struct aiScene;
class Renderer;
struct Vertex;



// Set of meshes with textures already applied in a 3D Software
class Model
{
public:
	Model(const std::filesystem::path& inPath, const bool inGammaCorrection = false);

	void StoreInstanceModelMatrices(const std::vector<glm::mat4>& modelMatrices, const std::size_t sizeInBytes) const;

	void Render(const Renderer& renderer) const;
	void RenderInstances(const Renderer& renderer, const uint32_t instanceCount) const;

	const std::vector<Texture>& GetTextures() const { return textures; }

private:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;

	[[maybe_unused]] bool gammaCorrection{ false };

	// Load a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector
	void LoadModel(const std::filesystem::path& path);

	// Process an ASSIMP mesh node recursively by transferring mesh data to Vertex-compatible vector
	void ProcessMeshNode(const aiNode& node, const aiScene& scene);

	void ProcessMesh(const aiMesh& mesh);

	// Retrieve ASSIMP vertex data and build a C++-compatible vector of Vertex instances out of it
	std::vector<Vertex> ProcessMeshVertices(const aiMesh& mesh);

	// Retrieve ASSIMP facet data and build a C++-compatible vector of ints out of it
	std::vector<uint32_t> ProcessMeshIndices(const aiMesh& mesh);

	// Retrieve ASSIMP texture data and build a C++-compatible vector of Texture instances out of it
	void ProcessTextures(const aiMesh& mesh, const aiScene& scene);
};



#endif // MODEL_H