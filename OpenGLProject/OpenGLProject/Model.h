#ifndef MODEL_H
#define MODEL_H

#include <assimp/material.h>
#include <glm/ext/matrix_transform.hpp>
#include <string>
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
	Model(const std::string& inPath, const bool inGammaCorrection = false);

	void StoreInstanceModelMatrices(const std::vector<glm::mat4>& modelMatrices, const size_t size) const;

	// @todo - Move render method implementations in SceneEntity child classes to be created for SaturnRings
	void Render(const Renderer& renderer, const uint32_t textureUnit) const;
	void RenderInstances(const Renderer& renderer, const uint32_t textureUnit, const uint32_t instanceCount) const;

private:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;

	bool gammaCorrection{ false };

	// Load a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector
	void LoadModel(const std::string& path);

	// Process an ASSIMP node recursively
	void ProcessNode(const aiNode& node, const aiScene& scene);
	Mesh ProcessMesh(const aiMesh& mesh, const aiScene& scene);

	// Store all vertices in a proper vector of Vertex instances (note ASSIMP does not use glm::vec3)
	std::vector<Vertex> GetMeshVertices(const aiMesh& mesh);

	// Store the indices of each mesh facet in a proper vector
	std::vector<uint32_t> GetMeshIndices(const aiMesh& mesh);

	// Create a Texture instance from each ASSIMP texture referenced in the model and store it in a proper vector
	void GetMeshTextures(const aiMesh& mesh, const aiScene& scene);
};



#endif // MODEL_H