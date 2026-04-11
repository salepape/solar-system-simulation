#ifndef MESH_H
#define MESH_H

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

class IndexBuffer;
class Renderer;
class VertexArray;
class VertexBuffer;



struct Vertex
{
	glm::vec3 position{ 0.0f };
	glm::vec3 normal{ 0.0f };
	glm::vec2 texCoords{ 0.0f };
	glm::vec3 tangent{ 0.0f };
	glm::vec3 biTangent{ 0.0f };

	static constexpr uint32_t POSITION_ELMTS_COUNT = 3;
	static constexpr uint32_t NORMAL_ELMTS_COUNT = 3;
	static constexpr uint32_t TEXCOORDS_ELMTS_COUNT = 2;
	static constexpr uint32_t TANGENT_ELMTS_COUNT = 3;
	static constexpr uint32_t BITANGENT_ELMTS_COUNT = 3;

	static constexpr uint32_t INSTANCE_MATRIX_ELMTS_COUNT = 4;
};

// Geometry and its associated buffer objects
class MeshComponent
{
public:
	// Default constructor (used when creating Mesh data from child classes, as vertices/indices can only be computed with child class params)
	MeshComponent() = default;

	// User-defined constructor (used when parsing a pre-made 3D model, i.e. a mesh with textures applied on it, and transferring Mesh info to this class) 
	MeshComponent(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices = {});

	// Copy constructor (needed when defining a Sphere in CelestialBody when object passed as const ref to instantiate Orbit/Billboard in BodySystem)
	MeshComponent(const MeshComponent& inMesh) = default;
	const MeshComponent& operator = (const MeshComponent& inMesh) = delete;

	// Move constructor (used when reading data from model file)
	MeshComponent(MeshComponent&& inMesh) = default;
	MeshComponent&& operator = (MeshComponent&& inMesh) = delete;

	// Virtual destructor (needed, as class is not final)
	virtual ~MeshComponent() = default;

	void StoreInstanceModelMatrices(const VertexBuffer& vbo) const;

	virtual void Render(const Renderer& renderer) const;
	void RenderInstances(const Renderer& renderer, const uint32_t instanceCount) const;

protected:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	std::shared_ptr<VertexArray> vao;
	std::shared_ptr<IndexBuffer> ibo;

	// Set vertex buffers and its attribute pointers once we have all required data
	void StoreVertices();
};



#endif // MESH_H