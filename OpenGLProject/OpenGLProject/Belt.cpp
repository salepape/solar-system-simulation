#include "Belt.h"

#include <glfw3.h>

#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



Belt::Belt(const Model inInstanceModel, const unsigned int inInstanceModelNb, const int inSizeFactor, const float inMajorRadius, const float inMinorRadius) :
	instanceModel(inInstanceModel), instanceModelNb(inInstanceModelNb), sizeFactor(inSizeFactor), majorRadius(inMajorRadius), minorRadius(inMinorRadius)
{
	Compute();
	Store();
}

Belt::~Belt()
{
	vao.~VertexArray();
}

void Belt::Compute()
{
	// Initialise random seed
	srand(static_cast<unsigned int>(glfwGetTime()));

	// Compute random position for each model instance of the belt tore
	for (unsigned int i = 0; i < instanceModelNb; ++i)
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		const float angle = static_cast<float>(i) / static_cast<float>(instanceModelNb) * 360.0f;

		const float xOffset = (rand() % static_cast<int>(2 * minorRadius * 100)) / 100.0f - minorRadius;
		const float x = sin(angle) * majorRadius + xOffset;

		// keep height of model field smaller compared to width of x and z
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
	VertexBuffer vbo(&modelMatrices[0], instanceModelNb * sizeof(glm::mat4));

	// Set transformation matrices as an instance vertex attribute (with divisor 1)
	for (unsigned int i = 0; i < instanceModel.GetMeshes().size(); ++i)
	{
		// Retrieve VAO ID of the rock mesh (we don't create any new VAO ID per belt because of instancing)
		vao = instanceModel.GetMeshes()[i].GetVaoRef();
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

void Belt::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	if (instanceModel.GetTextures().empty() == false)
	{
		// Note that we are assuming that a single texture will be stored in the model
		instanceModel.GetTextures()[0].Enable(textureUnit);
	}

	for (unsigned int i = 0; i < instanceModel.GetMeshes().size(); ++i)
	{
		renderer.DrawInstanced(vao, instanceModel.GetMeshes()[i].GetIndicesCount(), instanceModelNb);
	}
}


