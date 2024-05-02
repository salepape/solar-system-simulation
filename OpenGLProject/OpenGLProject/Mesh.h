#ifndef MESH_H
#define MESH_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <vector>

#include "Mesh.h"
#include "Texture.h"

class IndexBuffer;
class Renderer;
enum class TextureType;
class VertexArray;
class VertexBuffer;



struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	static constexpr int POSITION_ELMTS_COUNT = 3;
	static constexpr int NORMAL_ELMTS_COUNT = 3;
	static constexpr int TEXCOORDS_ELMTS_COUNT = 2;
	static constexpr int TANGENT_ELMTS_COUNT = 3;
	static constexpr int BITANGENT_ELMTS_COUNT = 3;

	static constexpr int INSTANCE_MATRIX_ELMTS_COUNT = 4;
};

class Mesh
{
public:
	// Default constructor needed for Skybox class constructor not using Mesh user-defined one
	Mesh() = default;
	// User-defined constructor used when computing mesh vertices/texture coordinates from scratch
	Mesh(const std::string& texturePath, const uint32_t textureTarget, const WrapOptions& textureWrapOptions, const FilterOptions& textureFilterOptions, const TextureType& textureType);
	// User-defined constructor used when parsing a pre-made 3D model (i.e. a mesh with textures applied on it) 
	Mesh(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices, const std::vector<Texture>& inTextures);
	~Mesh();

	void StoreModelMatrices(const VertexBuffer& vbo) const;

	virtual void Render(const Renderer& renderer, const uint32_t textureUnit) const;
	void RenderInstances(const Renderer& renderer, const uint32_t instanceCount) const;

protected:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;

	VertexArray* vao{ nullptr };
	IndexBuffer* ibo{ nullptr };

	// Compute each vertex of the mesh mathematically in code
	virtual void ComputeVertices() {};

	// Set vertex buffers and its attribute pointers once we have all required data
	virtual void StoreVertices();
};



#endif // MESH_H