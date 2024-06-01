#ifndef MESH_H
#define MESH_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Renderer;
class VertexBuffer;



struct Vertex
{
	glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 Normal{ 0.0f, 0.0f, 0.0f };
	glm::vec2 TexCoords{ 0.0f, 0.0f };
	glm::vec3 Tangent{ 0.0f, 0.0f, 0.0f };
	glm::vec3 Bitangent{ 0.0f, 0.0f, 0.0f };

	static constexpr uint32_t POSITION_ELMTS_COUNT = 3;
	static constexpr uint32_t NORMAL_ELMTS_COUNT = 3;
	static constexpr uint32_t TEXCOORDS_ELMTS_COUNT = 2;
	static constexpr uint32_t TANGENT_ELMTS_COUNT = 3;
	static constexpr uint32_t BITANGENT_ELMTS_COUNT = 3;

	static constexpr uint32_t INSTANCE_MATRIX_ELMTS_COUNT = 4;
};

// Geometry and its associated buffer objects
class Mesh
{
public:
	// Default constructor needed when computing mesh coordinates in code
	Mesh() = default;
	// User-defined constructor used when parsing a pre-made 3D model (i.e. a mesh with textures applied on it) 
	Mesh(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices);
	~Mesh() = default;

	void StoreInstanceModelMatrices(const VertexBuffer& vbo) const;

	virtual void Render(const Renderer& renderer) const;
	void RenderInstances(const Renderer& renderer, const uint32_t instanceCount) const;

protected:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	std::shared_ptr<VertexArray> vao;
	std::shared_ptr<IndexBuffer> ibo;

	// Compute each vertex of the mesh mathematically in code
	virtual void ComputeVertices() {};

	// Set vertex buffers and its attribute pointers once we have all required data
	virtual void StoreVertices();
};



#endif // MESH_H