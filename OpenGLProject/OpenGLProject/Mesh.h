#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> 
#include <glm/glm.hpp>	
#include <string>
#include <vector>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"



struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	static constexpr int GetPositionNumElmts() { return 3; }
	static constexpr int GetNormalNumElmts() { return 3; }
	static constexpr int GetTexCoordsNumElmts() { return 2; }
	static constexpr int GetTangentNumElmts() { return 3; }
	static constexpr int GetBitangentNumElmts() { return 3; }
};

class Mesh
{
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

public:
	// Built in code
	Mesh();
	// Built by parsing the model coming from a 3D modeling software
	Mesh(const std::vector<Vertex> verticesArg, const std::vector<unsigned int> indicesArg, const std::vector<Texture> texturesArg);
	virtual ~Mesh();

	virtual void Render(const Renderer& renderer, const unsigned int& textureUnit);

	inline VertexArray& GetVaoRef() const { return *vao; };
	inline unsigned int GetIndicesCount() const { return indices.size(); }
};



#endif // MESH_H