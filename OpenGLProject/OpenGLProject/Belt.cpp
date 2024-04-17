#include "Belt.h"

#include <algorithm>
#include <glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



Belt::Belt(const InstanceParams& inInstanceParams, const TorusParams& inTorusParams) :
	instanceParams(inInstanceParams), torusParams(inTorusParams)
{
	ComputeModelMatrices();
	StoreModelMatrices();
}

Belt::~Belt()
{

}

void Belt::ComputeModelMatrices()
{
	const float angleValue = 1.0f / static_cast<float>(instanceParams.count) * 360.0f;
	
	const float lowerBoundOffset = - torusParams.minorRadius;
	const float upperBoundOffset = torusParams.minorRadius + 1.0f;
	const int rangeSpanOffset = static_cast<int>(upperBoundOffset - lowerBoundOffset);

	// Initialise random seed
	srand(static_cast<unsigned int>(glfwGetTime()));

	modelMatrices.reserve(instanceParams.count);
	for (unsigned int i = 0; i < instanceParams.count; ++i)
	{
		glm::mat4 modelMatrix(1.0f);

		const float angle = static_cast<float>(i) * angleValue;

		const float xOffset = lowerBoundOffset + static_cast<float>(rand() % rangeSpanOffset);
		const float x = sin(angle) * torusParams.majorRadius + xOffset;

		// Keep height of model field smaller compared to width of x and z
		const float yOffset = lowerBoundOffset + static_cast<float>(rand() % rangeSpanOffset);
		const float y = yOffset * torusParams.flatnessFactor;

		const float zOffset = lowerBoundOffset + static_cast<float>(rand() % rangeSpanOffset);
		const float z = cos(angle) * torusParams.majorRadius + zOffset;

		// Move instance along circle of radius majorRadius in [-minorRadius, minorRadius]
		// (added the range lower bound to the random number modulo by the range span to get a random value in range [lowerBound, lowerBound + rangeSpan])
		modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));

		// Resize instance in range [sizeRangeLowerBound, "sizeRangeLowerBound + 0.sizeRangeSpan"]
		const float scale = instanceParams.sizeRangeLowerBound + 0.01f * static_cast<float>(rand() % instanceParams.sizeRangeSpan);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

		// Rotate instance by num degrees in range [0, 360] around a pre-determined axis
		const float rotAngle = static_cast<float>(rand() % 361);
		modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// Add current model matrix to the list
		modelMatrices.push_back(modelMatrix);
	}
}

void Belt::StoreModelMatrices()
{
	// Configure instanced array
	VertexBuffer vbo(static_cast<const void*>(modelMatrices.data()), static_cast<size_t>(instanceParams.count * sizeof(glm::mat4)));

	// Set transformation matrices as an instance vertex attribute for each mesh VAO already created
	for (const auto& mesh: instanceParams.model.GetMeshes())
	{
		VertexBufferLayout vbl;
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol1, GL_FLOAT, INSTANCE_MATRIX_ELMTS_COUNT);
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol2, GL_FLOAT, INSTANCE_MATRIX_ELMTS_COUNT);
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol3, GL_FLOAT, INSTANCE_MATRIX_ELMTS_COUNT);
		vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol4, GL_FLOAT, INSTANCE_MATRIX_ELMTS_COUNT);
		mesh.GetVAO().AddInstancedBuffer(vbo, vbl);
	}

	vbo.Unbind();
}

void Belt::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	for (const auto& texture : instanceParams.model.GetTextures())
	{
		texture.Enable(textureUnit);
	}

	for (const auto& mesh: instanceParams.model.GetMeshes())
	{
		renderer.DrawInstances(mesh.GetVAO(), static_cast<unsigned int>(mesh.GetIndicesCount()), instanceParams.count);
	}

	for (const auto& texture : instanceParams.model.GetTextures())
	{
		texture.Disable();
	}
}