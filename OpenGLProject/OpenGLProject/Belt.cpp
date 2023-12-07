#include "Belt.h"

#include <glfw3.h>

#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



Belt::Belt(const Model asteroidArg, const unsigned int asteroidNbArg, const int sizeFactorArg, const float majorRadiusArg, const float minorRadiusArg) :
	asteroid(asteroidArg), asteroidNb(asteroidNbArg), sizeFactor(sizeFactorArg), majorRadius(majorRadiusArg), minorRadius(minorRadiusArg)
{
	Compute();
	Store();
}

void Belt::Compute()
{
	modelMatrices = new glm::mat4[asteroidNb];

	// Initialise random seed
	srand(static_cast<unsigned int>(glfwGetTime()));

	for (unsigned int i = 0; i < asteroidNb; ++i)
	{
		glm::mat4 model = glm::mat4(1.0f);

		// Compute random position within the belt tore
		const float angle = static_cast<float>(i) / static_cast<float>(asteroidNb) * 360.0f;
		const float displacement_x = (rand() % static_cast<int>(2 * minorRadius * 100)) / 100.0f - minorRadius;
		const float x = sin(angle) * majorRadius + displacement_x;
		const float displacement_y = (rand() % static_cast<int>(2 * minorRadius * 100)) / 100.0f - minorRadius;
		const float y = displacement_y * 0.4f; // keep height of asteroid field smaller compared to width of x and z
		const float displacement_z = (rand() % static_cast<int>(2 * minorRadius * 100)) / 100.0f - minorRadius;
		const float z = cos(angle) * majorRadius + displacement_z;
		model = glm::translate(model, glm::vec3(x, y, z));

		// Resize between 0.05 and "0.05 + 0.sizeFactor"
		const float scale = static_cast<float>(rand() % sizeFactor) / 100.0f + 0.05f;
		model = glm::scale(model, glm::vec3(scale));

		// Add random rotation around a (semi)-randomly picked rotation axis vector
		const float rotAngle = static_cast<float>(rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// Add current model matrix to the list
		modelMatrices[i] = model;
	}
}

// We could instantiate several belts with the same model (ie its VAO ID) if modelMatrices gathered data of all belts
void Belt::Store()
{
	// Configure instanced array
	VertexBuffer vbo(modelMatrices, asteroidNb * sizeof(glm::mat4));

	// Set transformation matrices as an instance vertex attribute (with divisor 1)
	for (unsigned int i = 0; i < asteroid.GetMeshes().size(); ++i)
	{
		// Retrieve VAO ID of the rock mesh (we don't create any new VAO ID per belt because of instancing)
		vao = asteroid.GetMeshes()[i].GetVaoRef();
		vao.Bind();

		constexpr int NbElements = GetInstanceMatrixNumElmts();

		VertexBufferLayout vbl;
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol1, GL_FLOAT, NbElements);
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol2, GL_FLOAT, NbElements);
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol3, GL_FLOAT, NbElements);
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol4, GL_FLOAT, NbElements);
		vao.AddInstancedBuffer(vbo, vbl);

		vao.Unbind();
	}
}

Belt::~Belt()
{
	vao.~VertexArray();
}

void Belt::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	if (asteroid.GetTextures().empty() == false)
	{
		// Note that we are assuming that a single texture will be stored in the asteroid model
		asteroid.GetTextures()[0].Enable(textureUnit);
	}

	for (unsigned int i = 0; i < asteroid.GetMeshes().size(); ++i)
	{
		renderer.DrawInstanced(vao, asteroid.GetMeshes()[i].GetIndicesCount(), asteroidNb);
	}
}


