#include "BeltEntity.h"

#include <cstdlib> // std::srand() and std::rand()
#include <cstddef> // std::size_t
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <utility>

#include "Application.h"
#include "Renderer.h"
#include "Shader.h"
#include "ShaderLoader.h"



BeltEntity::BeltEntity(const std::string& inName, InstanceParams&& inInstanceParams, TorusParams&& inTorusParams) : SceneEntity(inName),
instanceParams(inInstanceParams), torusParams(inTorusParams), model(inInstanceParams.modelPath, ShaderLookUpID::Enum::BELT)
{
	ComputeInstanceModelMatrices();
	StoreInstanceModelMatrices();
}

void BeltEntity::ComputeInstanceModelMatrices()
{
	const float angleValue = 1.0f / instanceParams.count * 360.0f;

	const float lowerBoundOffset = -torusParams.minorRadius;
	const float upperBoundOffset = torusParams.minorRadius + 1.0f;
	const int32_t rangeSpanOffset = static_cast<int32_t>(upperBoundOffset - lowerBoundOffset);

	// Initialise random seed
	std::srand(static_cast<uint32_t>(Application::GetInstance().GetTime()));

	modelMatrices.reserve(instanceParams.count);
	for (uint32_t i = 0; i < instanceParams.count; ++i)
	{
		glm::mat4 modelMatrix(1.0f);

		const float angle = i * angleValue;

		const float xOffset = lowerBoundOffset + static_cast<float>(std::rand() % rangeSpanOffset);
		const float x = glm::sin(angle) * torusParams.majorRadius + xOffset;

		// Keep height of model field smaller compared to width of x and z
		const float yOffset = lowerBoundOffset + static_cast<float>(std::rand() % rangeSpanOffset);
		const float y = yOffset * torusParams.flatnessFactor;

		const float zOffset = lowerBoundOffset + static_cast<float>(std::rand() % rangeSpanOffset);
		const float z = glm::cos(angle) * torusParams.majorRadius + zOffset;

		// Move instance along circle of radius majorRadius in [-minorRadius, minorRadius]
		// (added the range lower bound to the random number modulo by the range span to get a random value in range [lowerBound, lowerBound + rangeSpan])
		modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));

		// Resize instance in range [sizeRangeLowerBound, "sizeRangeLowerBound + 0.sizeRangeSpan"]
		const float scale = instanceParams.sizeRangeLowerBound + 0.01f * static_cast<float>(std::rand() % instanceParams.sizeRangeSpan);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

		// Rotate instance by num degrees in range [0, 360] around a pre-determined axis
		const float rotAngle = static_cast<float>(std::rand() % 361);
		modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// Add current model matrix to the list
		modelMatrices.push_back(std::move(modelMatrix));
	}
}

void BeltEntity::StoreInstanceModelMatrices() const
{
	model.StoreInstanceModelMatrices(modelMatrices, static_cast<std::size_t>(instanceParams.count) * sizeof(glm::mat4));
}

void BeltEntity::Render(const Renderer& renderer, const float /*elapsedTime*/)
{
	const Material& modelMaterial = model.GetMaterials()[0];
	const Shader& shader = modelMaterial.GetShader();
	shader.Enable();

	modelMaterial.EnableTextures();
	model.RenderInstances(renderer, instanceParams.count);
	modelMaterial.DisableTextures();

	shader.Disable();
}