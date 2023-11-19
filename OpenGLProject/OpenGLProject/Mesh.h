#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> 
#include <glm/glm.hpp>	
#include <string>
#include <vector>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArray.h"



struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	VertexArray* vao{ nullptr };
	VertexBuffer* vbo{ nullptr };
	IndexBuffer* ibo{ nullptr };

	void Store();

public:
	Mesh(const std::vector<Vertex> verticesArg, const std::vector<unsigned int> indicesArg, const std::vector<Texture> texturesArg);
	~Mesh();

	void Render(const Renderer& renderer, const unsigned int& textureUnit);

	inline VertexArray& GetVaoRef() const { return *vao; };
	inline unsigned int GetIndicesCount() const { return indices.size(); }
};



#endif // MESH_H