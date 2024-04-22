#ifndef MODEL_H
#define MODEL_H

#include <assimp/material.h>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Mesh.h"
#include "Texture.h"

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;
class Renderer;
struct Vertex;



class Model
{
public:
	Model();
	Model(const std::string& inPath, const bool inGammaCorrection = false);
	~Model();

	void StoreModelMatrices(const std::vector<glm::mat4>& modelMatrices, const size_t size);

	void Render(const Renderer& renderer, const unsigned int textureUnit);
	void RenderInstances(const Renderer& renderer, const unsigned int textureUnit, const unsigned int instanceCount);

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
	std::vector<unsigned int> GetMeshIndices(const aiMesh& mesh);

	// Create a Texture instance from each ASSIMP texture referenced in the model and store it in a proper vector
	void GetMeshTextures(const aiMesh& mesh, const aiScene& scene);
};



#endif // MODEL_H