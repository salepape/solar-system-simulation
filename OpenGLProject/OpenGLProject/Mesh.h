#ifndef MESH_H
#define MESH_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <vector>

enum class GeometryType;
class IndexBuffer;
enum class MapType;
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

	static constexpr int POSITION_ELMTS_COUNT = 3;
	static constexpr int NORMAL_ELMTS_COUNT = 3;
	static constexpr int TEXCOORDS_ELMTS_COUNT = 2;
	static constexpr int TANGENT_ELMTS_COUNT = 3;
	static constexpr int BITANGENT_ELMTS_COUNT = 3;
};

class Mesh
{
public:
	Mesh();
	Mesh(const std::string& texturePath, int textureType, GeometryType geometryType, MapType mapType);
	// Built by parsing the model coming from a 3D modeling software
	Mesh(const std::vector<Vertex>& inVertices, const std::vector<unsigned int>& inIndices, const std::vector<Texture>& inTextures);
	virtual ~Mesh();

	virtual void Render(const Renderer& renderer, const unsigned int& textureUnit);

	VertexArray& GetVAO() const { return *vao; };
	size_t GetIndicesCount() const { return indices.size(); }

protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	VertexArray* vao{ nullptr };
	IndexBuffer* ibo{ nullptr };

	// Compute each vertex of the mesh mathematically in code
	virtual void ComputeVertices();

	// Set vertex buffers and its attribute pointers once we have all required data
	virtual void StoreVertices();
};



#endif // MESH_H