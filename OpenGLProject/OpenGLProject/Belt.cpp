#include "Belt.h"



Belt::Belt(Model asteroidArg, unsigned int asteroidNbArg, int sizeFactorArg, float majorRadiusArg, float minorRadiusArg) :
	asteroid(asteroidArg), asteroidNb(asteroidNbArg), sizeFactor(sizeFactorArg), majorRadius(majorRadiusArg), minorRadius(minorRadiusArg)
{
	Compute();
	Store();
}

void Belt::Compute()
{
	modelMatrices = new glm::mat4[asteroidNb];

	srand((unsigned int)glfwGetTime());					// Initialize random seed	

	for (unsigned int i = 0; i < asteroidNb; ++i)
	{
		glm::mat4 model = glm::mat4(1.0f);

		// TRANSLATION : compute random position within the belt tore
		float angle = (float)i / (float)asteroidNb * 360.0f;
		float displacement = (rand() % (int)(2 * minorRadius * 100)) / 100.0f - minorRadius;
		float x = sin(angle) * majorRadius + displacement;
		displacement = (rand() % (int)(2 * minorRadius * 100)) / 100.0f - minorRadius;
		float y = displacement * 0.4f;					// keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * minorRadius * 100)) / 100.0f - minorRadius;
		float z = cos(angle) * majorRadius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// SCALE : resize between 0.05 and "0.05 + 0.sizeFactor"
		float scale = (float)(rand() % sizeFactor) / 100.0f + 0.05f;
		model = glm::scale(model, glm::vec3(scale));

		// ROTATION : add random rotation around a (semi)-randomly picked rotation axis vector
		float rotAngle = (float)(rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// Add current model matrix to the list
		modelMatrices[i] = model;
	}
}

void Belt::Store()
{
	// Configure instanced array
	VertexBuffer vbo(modelMatrices, asteroidNb * sizeof(glm::mat4));

	// Set transformation matrices as an instance vertex attribute (with divisor 1)
	for (unsigned int i = 0; i < asteroid.meshes.size(); ++i)
	{
		vao = asteroid.meshes[i].GetVaoRef();		// TO SOLVE : Kuiper overwrites asteroid belt data !!!
		vao.Bind();

		VertexBufferLayout vbl;
		vbl.Push<float>(4);
		vbl.Push<float>(4);
		vbl.Push<float>(4);
		vbl.Push<float>(4);
		vao.AddInstancedBuffer(vbo, vbl);		

		vao.Unbind();
	}
}

Belt::~Belt()
{

}

void Belt::Render()
{
	for (unsigned int i = 0; i < asteroid.meshes.size(); ++i)
	{
		vao.Bind();
		glDrawElementsInstanced(GL_TRIANGLES, asteroid.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, asteroidNb);
		vao.Unbind();
	}
}


