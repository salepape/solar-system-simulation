#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

#include <vector>

class Sphere
{
private:
	// Number of bytes encoding vertCoor vector
	int sizeofVertices;
	// Number of double values that contains indexes vector
	int nbIndexes;
	// Vertex Array Object
	unsigned int VAO;
	// Vertex Buffer Object
	unsigned int VBO;
	// Element Buffer Object
	unsigned int EBO;



public:
	Sphere(float radius)
	{
		// ZONE  = triangle or square formed by intersection of 1 meridian strip and 1 parallel strip
		// THETA = ANGLE BETWEEN 2 ZONES OF ONE PARALLEL STRIP; 
		// PHI   = ANGLE BETWEEN 2 ZONES OF ONE MERIDIAN STRIP; 
		//-> using spherical coordinate system
		float pi = glm::pi<float>(), theta, cosTheta, phi, zCoor;
		int nbParalStrips = 100, nbMeridStrips = 100;
		std::vector<float> vertCoor, textCoor;
		std::vector<int> indexes;

		for (int i = 0; i <= nbParalStrips; ++i)
		{
			theta = pi * (0.5f - (float)i / nbParalStrips);
			cosTheta = glm::cos(theta);
			zCoor = radius * glm::sin(theta);

			for (int j = 0; j <= nbMeridStrips; ++j)
			{
				phi = 2.0f * pi * (float)j / nbMeridStrips;

				// Poisition of the current vertex 
				vertCoor.push_back(radius * cosTheta * glm::cos(phi));
				vertCoor.push_back(radius * cosTheta * glm::sin(phi));
				vertCoor.push_back(zCoor);

				// Texture coordinates of the current vertex
				textCoor.push_back((float)j / nbMeridStrips);
				textCoor.push_back((float)i / nbParalStrips);
			}
		}

		// indices
		//  k1--k1+1
		//  |  / |
		//  | /  |
		//  k2--k2+1
		unsigned int index1, index2;
		for (int i = 0; i < nbParalStrips; ++i)
		{
			index1 = i * (nbMeridStrips + 1);			// beginning of current parallel strip
			index2 = index1 + nbMeridStrips + 1;		// beginning of next parallel strip

			for (int j = 0; j < nbMeridStrips; ++j, ++index1, ++index2)
			{
				// 2 triangles per zone except for those formed by first (top) and last (bottom) parallel strips
				if (i != 0)
				{
					indexes.push_back(index1);
					indexes.push_back(index2);
					indexes.push_back(index1 + 1);		// k1---k2---k1+1
				}

				if (i != (nbParalStrips - 1))
				{
					indexes.push_back(index1 + 1);
					indexes.push_back(index2);
					indexes.push_back(index2 + 1);		// k1+1---k2---k2+1
				}
			}
		}

		sizeofVertices = sizeof(float) * vertCoor.size();
		int sizeofTextures = sizeof(float) * textCoor.size();
		int sizeofIndexes = sizeof(unsigned int) * indexes.size();

		nbIndexes = indexes.size();





		// GENERATION VAO - Generate 1 VAO storing its ID / name in the provided array within graphics memory
		glGenVertexArrays(1, &VAO);
		// BINDING VAO - Bind VAO, which thus will store all following VBO calls
		glBindVertexArray(VAO);

		// GENERATION VBO - Generate 1 BO storing its ID / name in the provided array within graphics memory
		glGenBuffers(1, &VBO);
		// BINDING VBO - Bind BO to the OpenGL BO target we want (here the one of a vertex, called GL_ARRAY_BUFFER, hence VBO variable name)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// COPY - Allocate memory and store data into the currently bound BO memory
		glBufferData(GL_ARRAY_BUFFER, sizeofVertices + sizeofTextures, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeofVertices, &vertCoor[0]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeofVertices, sizeofTextures, &textCoor[0]);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofIndexes, &indexes[0], GL_STATIC_DRAW);

		// INTERPRETATION - Store in the currently bound VBO how OpenGL should interpret the vertex buffer data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)sizeofVertices);
	}

	void Draw()
	{
		// PERMISSION - Enable the vertex attribute provided its location
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, nbIndexes, GL_UNSIGNED_INT, (void*)0);
	}

	~Sphere()
	{
		// CLEAN VBO (optional) - Free the resource once it've outlived its purpose
		glDeleteBuffers(1, &EBO);

		// CLEAN VBO (optional) - Free the resource once it've outlived its purpose
		glDeleteBuffers(1, &VBO);

		// CLEAN VAO (optional) - Free the resource once it've outlived its purpose
		glDeleteVertexArrays(1, &VAO);
	}
};

#endif SPHERE_H