#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cstdint>
#include <memory>
#include <vector>

class IndexBuffer;
class VertexArray;
class VertexBuffer;



struct Vertex
{
	glm::vec3 position{ 0.0f };
	glm::vec3 normal{ 0.0f };
	glm::vec2 texCoords{ 0.0f };
	glm::vec3 tangent{ 0.0f };
	glm::vec3 biTangent{ 0.0f };

	static constexpr uint32_t POSITION_TYPE_DIMENSION = 3;
	static constexpr uint32_t NORMAL_TYPE_DIMENSION = 3;
	static constexpr uint32_t TEXCOORDS_TYPE_DIMENSION = 2;
	static constexpr uint32_t TANGENT_TYPE_DIMENSION = 3;
	static constexpr uint32_t BITANGENT_TYPE_DIMENSION = 3;

	static constexpr uint32_t INSTANCE_MATRIX_COL_TYPE_DIMENSION = 4;
};

// 3D Geometry and its associated buffer objects
class MeshComponent
{
public:
	// Default constructor (used when creating Mesh data from child classes, as vertices/indices can only be computed with child class params)
	MeshComponent() = default;

	// User-defined constructor (used when parsing a pre-made 3D model, i.e. a mesh with textures applied on it, and transferring Mesh info to this class) 
	MeshComponent(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices = {});

	// Copy constructor (not needed - automatically deleted since the class holds a std::unique_ptr)
	MeshComponent(const MeshComponent& inMesh) = delete;
	const MeshComponent& operator = (const MeshComponent& inMesh) = delete;

	// Move constructor (used when reading data from model file)
	MeshComponent(MeshComponent&& inMesh) = default;
	MeshComponent&& operator = (MeshComponent&& inMesh) = delete;

	// Virtual destructor (needed, as class is not final)
	virtual ~MeshComponent() = default;

	void StoreInstanceTransforms() const;

	// Call the appropriate OpenGL draw function according to the emptiness of the indices vector
	virtual void Render(const unsigned int mode = GL_TRIANGLES) const;
	virtual void RenderInstances(const uint32_t instanceCount) const;

protected:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	// As Vertex Array destructor is called by unique_ptr at some point in Mesh Component source file, 
	// and Vertex Array type is incomplete at this point (forward-declared), we have to implement a custom destructor
	struct VertexArrayDeleter
	{
		void operator()(VertexArray* ptr) noexcept;
	};
	std::unique_ptr<VertexArray, VertexArrayDeleter> vao;

	// As Index Buffer destructor is called by unique_ptr at some point in Mesh Component source file, 
	// and Index Buffer type is incomplete at this point (forward-declared), we have to implement a custom destructor
	struct IndexBufferDeleter
	{
		void operator()(IndexBuffer* ptr) noexcept;
	};
	std::unique_ptr<IndexBuffer, IndexBufferDeleter> ibo;

	// Set vertex buffers and its attribute pointers once we have all required data
	void StoreVertices();

private:
	bool IsIndicesBuffer() const { return indices.empty() == false; }
};



#endif // MESH_H
