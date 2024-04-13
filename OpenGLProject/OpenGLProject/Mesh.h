#ifndef MESH_H
#define MESH_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <vector>

class IndexBuffer;
class Renderer;
class Texture;
class VertexArray;
class VertexBuffer;



struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	static constexpr int GetPositionElmtsCount() { return 3; }
	static constexpr int GetNormalElmtsCount() { return 3; }
	static constexpr int GetTexCoordsElmtsCount() { return 2; }
	static constexpr int GetTangentElmtsCount() { return 3; }
	static constexpr int GetBitangentElmtsCount() { return 3; }
};

class Mesh
{
public:
	// Built in code
	Mesh();
	// Built by parsing the model coming from a 3D modeling software
	Mesh(const std::vector<Vertex>& inVertices, const std::vector<unsigned int>& inIndices, const std::vector<Texture>& inTextures);
	virtual ~Mesh();

	virtual void Render(const Renderer& renderer, const unsigned int& textureUnit);

	const VertexArray& GetVaoRef() const { return *vao; };
	size_t GetIndicesCount() const { return indices.size(); }

protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	VertexArray* vao{ nullptr };
	VertexBuffer* vbo{ nullptr };
	IndexBuffer* ibo{ nullptr };

	// Compute each vertex of the mesh mathematically in code
	virtual void Compute();

	// Set vertex buffers and its attribute pointers once we have all required data
	virtual void Store();
};



#endif // MESH_H