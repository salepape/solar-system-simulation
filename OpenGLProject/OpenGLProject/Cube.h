#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

#include <vector>

class Cube
{
private:
	// Dim
	int sizeofVertices;
	// Vertex Buffer Object
	unsigned int VBO;

public:
	Cube()
	{
		// 6 faces * 2 triangles * 3 vertices for one 3D cube 
		float vertCoor[] =
		{
			// Front face
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			// Back face
			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			// Left face
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			// Right face
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			 // Bottom face
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			// Top face
			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f
		};

		// 6 faces * 2 triangles * 2 texture coordinates for one 3D cube 
		float textCoor[] =
		{
			// Front face
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			// Back face
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			// Left face
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,

			// Right face
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,

			// Bottom face
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f,

			// Top face
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f
		};

		sizeofVertices = sizeof(vertCoor);

		// GENERATION VBO - Generate 1 BO storing its ID / name in the provided array within graphics memory
		glGenBuffers(1, &VBO);

		//// BINDING VAO - Bind VAO, which thus will store all following VBO calls
		//glBindVertexArray(VAO);

		// BINDING VBO - Bind BO to the OpenGL BO target we want (here the one of a vertex, called GL_ARRAY_BUFFER, hence VBO variable name)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// COPY - Allocate memory and store data into the currently bound BO memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertCoor) + sizeof(textCoor), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertCoor), &vertCoor);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertCoor), sizeof(textCoor), &textCoor);

		// INTERPRETATION - Store in the currently bound VBO how OpenGL should interpret the vertex buffer data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)sizeofVertices);
	};

	void Draw()
	{
		//// PERMISSION - Enable the vertex attribute provided its location
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	~Cube()
	{
		// CLEAN VBO (optional) - Free the resource once it've outlived its purpose
		glDeleteBuffers(1, &VBO);
	}
};

#endif CUBE_H