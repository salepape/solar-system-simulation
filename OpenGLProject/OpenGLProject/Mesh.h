#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> 

#include <glm/glm.hpp>	

#include <string>
#include <vector>

#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"



struct Vertex 
{
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// Texture coordinates
	glm::vec2 TexCoords;
	// Tangent
	glm::vec3 Tangent;
	// Bitangent
	glm::vec3 Bitangent;
};

class Mesh 
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	VertexArray * vao;
	VertexBuffer * vbo;
	IndexBuffer * ibo;

	void Store();

public:
	Mesh(std::vector<Vertex> verticesArg, std::vector<unsigned int> indicesArg, std::vector<Texture> texturesArg);
	~Mesh();

	//void Draw(ShaderProgram &shader);
	void Render();

	// Save reference of mesh vao to be used within belt.cpp
	inline VertexArray& GetVaoRef() const { return *vao; };
	inline int GetIndicesCount() const { return indices.size(); }
};
#endif