#include "Belt.h"

#include <glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



Belt::Belt(const Model inInstance, const unsigned int inInstancesCount, const int inSizeFactor, const float inMajorRadius, const float inMinorRadius) :
	instance(inInstance), instancesCount(inInstancesCount), sizeFactor(inSizeFactor), majorRadius(inMajorRadius), minorRadius(inMinorRadius)
{
	Compute();
	Store();
}

Belt::~Belt()
{

}

void Belt::Compute()
{
	// Initialise random seed
	srand(static_cast<unsigned int>(glfwGetTime()));

	// Compute random position for each model instance of the belt tore
	for (unsigned int i = 0; i < instancesCount; ++i)
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		const float angle = static_cast<float>(i) / static_cast<float>(instancesCount) * 360.0f;

		const float xOffset = (rand() % static_cast<int>(2 * minorRadius * 100)) / 100.0f - minorRadius;
		const float x = sin(angle) * majorRadius + xOffset;

		// Keep height of model field smaller compared to width of x and z
		const float yOffset = (rand() % static_cast<int>(2 * minorRadius * 100)) / 100.0f - minorRadius;
		const float y = yOffset * 0.4f;

		const float zOffset = (rand() % static_cast<int>(2 * minorRadius * 100)) / 100.0f - minorRadius;
		const float z = cos(angle) * majorRadius + zOffset;

		modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));

		// Resize between 0.05 and "0.05 + 0.sizeFactor"
		const float scale = static_cast<float>(rand() % sizeFactor) / 100.0f + 0.05f;
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

		// Add rotation around a randomly picked rotation axis vector
		const float rotAngle = static_cast<float>(rand() % 360);
		modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// Add current model matrix to the list
		modelMatrices.push_back(modelMatrix);
	}
}

void Belt::Store()
{
	// Configure instanced array
	VertexBuffer vbo(modelMatrices.data(), instancesCount * sizeof(glm::mat4));

	// Set transformation matrices as an instance vertex attribute (with divisor 1)
	for (unsigned int i = 0; i < instance.GetMeshes().size(); ++i)
	{
		// Retrieve VAO ID of the rock mesh (we don't create any new VAO ID per belt because of instancing)
		vao = instance.GetMeshes()[i].GetVaoRef();
		vao.Bind();

		constexpr int ElementsCount = GetInstanceMatrixElmtsCount();

		VertexBufferLayout vbl;
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol1, GL_FLOAT, ElementsCount);
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol2, GL_FLOAT, ElementsCount);
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol3, GL_FLOAT, ElementsCount);
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol4, GL_FLOAT, ElementsCount);
		vao.AddInstancedBuffer(vbo, vbl);

		vao.Unbind();
	}
}

void Belt::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	if (instance.GetTextures().empty() == false)
	{
		// Note that we are assuming that a single texture will be stored in the model
		instance.GetTextures()[0].Enable(textureUnit);
	}

	for (unsigned int i = 0; i < instance.GetMeshes().size(); ++i)
	{
		renderer.DrawInstanced(vao, static_cast<unsigned int>(instance.GetMeshes()[i].GetIndicesCount()), instancesCount);
	}
}