#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <glad/glad.h>
#include <glm/glm.hpp>



class Background
{
private:
	// Vertex Buffer Object
	unsigned int VBO;
	// Vertex Array Object
	unsigned int VAO;

public:
	Background()
	{
		float bgVertCoor[] =
		{
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
		};

		float bgTextCoor[] =
		{
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
		};

		int sizeofVertices = sizeof(bgVertCoor);
		int sizeofTextures = sizeof(bgTextCoor);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeofVertices + sizeofTextures, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeofVertices, &bgVertCoor[0]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeofVertices, sizeofTextures, &bgTextCoor[0]);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)sizeofVertices);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	};

	void Draw()
	{
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	~Background()
	{
		glDeleteBuffers(1, &VBO);

		glDeleteVertexArrays(1, &VAO);
	}
};

#endif BACKGROUND_H