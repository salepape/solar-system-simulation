#ifndef MODEL_H
#define MODEL_H

#include <cstddef> // std::size_t
#include <cstdint>
#include <filesystem>
#include <glm/mat4x4.hpp>
#include <vector>

#include "BlinnPhongMaterial.h"
#include "Mesh.h"
#include "ShaderLoader.h"
#include "Texture.h"

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;
class Renderer;
struct Vertex;



// Set of Meshes with Materials already applied from a 3D Software (e.g. Blender, Maya, etc.)
class Model
{
public:
	Model(const std::filesystem::path& inPath, const ShaderLookUpID::Enum inShaderLookUpID, const bool inGammaCorrection = false);

	void StoreInstanceModelMatrices(const std::vector<glm::mat4>& modelMatrices, const std::size_t sizeInBytes) const;

	void Render(const Renderer& renderer) const;
	void RenderInstances(const Renderer& renderer, const uint32_t instanceCount) const;

	const std::vector<Mesh>& GetMeshes() const { return meshes; }
	const std::vector<BlinnPhongMaterial>& GetMaterials() const { return materials; }

private:
	// List of Sub-Meshes that represents the Model
	// Warning: Model is supposed to be simple enough, i.e. only contains 1 Mesh, 1 Material definition, 1 Texture
	std::vector<Mesh> meshes;

	// List of Materials that applies to each Sub-Mesh of the Model (1 Sub-Mesh, 1 Material - as per ASSIMP import convention)
	// Owned by Model class since it's directly retrieved from .mtl file, not created from Scene Entity
	// Warning: Model is supposed to be simple enough, i.e. only contains 1 Mesh, 1 Material definition, 1 Texture
	std::vector<BlinnPhongMaterial> materials;
	ShaderLookUpID::Enum shaderLookUpID;

	[[maybe_unused]] bool gammaCorrection{ false };

	// Load a model from its 3D format using ASSIMP model importer (.obj verified, .gltf not tested) and stores the resulting data in Mesh/Material vectors
	void LoadModel(const std::filesystem::path& path);

	// Process an ASSIMP mesh node recursively by transferring mesh data to Vertex-compatible vector
	void ProcessMeshNode(const aiNode& node, const aiScene& scene);

	// Retrieve ASSIMP mesh data (in .obj Material file) and store Mesh instances in the associated vector
	void ProcessMesh(const aiMesh& mesh);

	// Retrieve ASSIMP vertex data (in .obj Material file) and build a vector of Vertex instances out of it
	std::vector<Vertex> ProcessMeshVertices(const aiMesh& mesh);

	// Retrieve ASSIMP index data (in .obj Material file) and build a vector of (Vertex) indexes out of it
	std::vector<uint32_t> ProcessMeshIndices(const aiMesh& mesh);

	// Retrieve ASSIMP material data (in .mtl Material companion file) and store Material instances in the associated vector
	void ProcessMaterial(const aiMesh& mesh, const aiScene& scene);

	// Retrieve ASSIMP texture data (in .mtl Material companion file) and build a vector of Texture instances out of it
	std::vector<Texture> ProcessTextures(const aiMaterial& material);
};



#endif // MODEL_H