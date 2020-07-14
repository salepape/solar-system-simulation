#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include "Texture.h"

#include <glad/glad.h> 

#include <glm/glm.hpp>	

#include <string>
#include <vector>



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
public:
	// Mesh Data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;
	unsigned int VAO;

	// Constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void Draw(Shader &shader);

private:
	// render data 
	unsigned int VBO, EBO;

	void setupMesh();
};
#endif