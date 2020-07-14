#include <glad/glad.h>
#include "Model.h"
#include "Data.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>



///////////////////////// BELTS /////////////////////////
struct Belt
{
	int asteroidNb;
	int sizeFactor;
	float majorRadius;
	float minorRadius;
};

int totalNbAsteroid;

void CreateBelts(Model asteroid)
{
	Belt asteroidBelt = { 5000, 10, data["Mars"].dist * 1.1f, 2.75f * DIST_SCALE_FACTOR / 2.5f };
	Belt kuiperBelt = { 20000, 20, data["Neptune"].dist * 1.4f, 30.05f * DIST_SCALE_FACTOR };
	static const int nbBelts = 2;
	Belt belts[nbBelts] = { asteroidBelt, kuiperBelt };

	// Generate large list of semi-random model transformation matrices, each representing an asteroid in a belt
	glm::mat4 * modelMatrices;
	totalNbAsteroid = asteroidBelt.asteroidNb + kuiperBelt.asteroidNb;
	modelMatrices = new glm::mat4[totalNbAsteroid];
	srand((unsigned int)glfwGetTime());					// Initialize random seed	

	int k = 0;
	for (int b = 0; b < nbBelts; ++b)
	{
		for (int i = k + 0; i < k + belts[b].asteroidNb; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			// TRANSLATION : compute random position within the belt tore
			float angle = (float)i / (float)belts[b].asteroidNb * 360.0f;
			float displacement = (rand() % (int)(2 * belts[b].minorRadius * 100)) / 100.0f - belts[b].minorRadius;
			float x = sin(angle) * belts[b].majorRadius + displacement;
			displacement = (rand() % (int)(2 * belts[b].minorRadius * 100)) / 100.0f - belts[b].minorRadius;
			float y = displacement * 0.4f;					// keep height of asteroid field smaller compared to width of x and z
			displacement = (rand() % (int)(2 * belts[b].minorRadius * 100)) / 100.0f - belts[b].minorRadius;
			float z = cos(angle) * belts[b].majorRadius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// SCALE : resize between 0.05 and "0.05 + 0.sizeFactor"
			float scale = (float)(rand() % belts[b].sizeFactor) / 100.0f + 0.05f;
			model = glm::scale(model, glm::vec3(scale));

			// ROTATION : add random rotation around a (semi)-randomly picked rotation axis vector
			float rotAngle = (float)(rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// Add current model matrix to the list
			modelMatrices[i] = model;
		}

		k = belts[b].asteroidNb;
	}

	// Configure instanced array
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, totalNbAsteroid * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	// Set transformation matrices as an instance vertex attribute (with divisor 1)
	for (unsigned int i = 0; i < asteroid.meshes.size(); ++i)
	{
		unsigned int VAO = asteroid.meshes[i].VAO;
		glBindVertexArray(VAO);

		// Set attribute pointers for matrix (4 times vec4 due to GLSL vertex shader limitation)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}

void RenderBelts(Model asteroid)
{
	for (unsigned int i = 0; i < asteroid.meshes.size(); ++i)
	{
		glBindVertexArray(asteroid.meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, asteroid.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, totalNbAsteroid);
		glBindVertexArray(0);
	}
}