#include "BeltEntity.h"

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include <cstdlib> // std::srand() and std::rand()
#include <cstddef> // std::size_t
#include <utility>

#include "Application/Application.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderLoader.h"



BeltEntity::BeltEntity(const std::string& inName, InstanceParams&& inInstanceParams, TorusParams&& inTorusParams) :
	SceneEntity(inName),
	instanceParams(inInstanceParams),
	torusParams(inTorusParams),
	model(inInstanceParams.modelPath, ShaderLookUpID::Enum::BELT)
{
	ComputeInstanceTransforms();
	StoreInstanceTransforms();
}

void BeltEntity::ComputeInstanceTransforms()
{
	const float angleValue = 1.0f / instanceParams.count * 360.0f;

	const float lowerBoundOffset = -torusParams.minorRadius;
	const float upperBoundOffset = torusParams.minorRadius + 1.0f;
	const int32_t rangeSpanOffset = static_cast<int32_t>(upperBoundOffset - lowerBoundOffset);

	// Initialise random seed
	std::srand(static_cast<uint32_t>(Application::GetInstance().GetElapsedTime()));

	transforms.reserve(instanceParams.count);
	for (uint32_t i = 0; i < instanceParams.count; ++i)
	{
		Transform transform;

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
		transform.Translate(glm::vec3(x, y, z));

		// Resize instance in range [sizeRangeLowerBound, "sizeRangeLowerBound + 0.sizeRangeSpan"]
		const float scale = instanceParams.sizeRangeLowerBound + 0.01f * static_cast<float>(std::rand() % instanceParams.sizeRangeSpan);
		transform.Scale(glm::vec3(scale));

		// Rotate instance by num degrees in range [0, 360] around a pre-determined axis
		const float rotAngle = static_cast<float>(std::rand() % 361);
		transform.Rotate(rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// Add current model matrix to the list
		transforms.push_back(std::move(transform));
	}
}

void BeltEntity::StoreInstanceTransforms() const
{
	model.StoreInstanceTransforms(transforms);
}

void BeltEntity::Render()
{
	const Material& modelMaterial = model.GetMaterials()[0];
	const Shader& shader = modelMaterial.GetShader();
	shader.Enable();

	modelMaterial.EnableTextures();
	model.RenderInstances(instanceParams.count);
	modelMaterial.DisableTextures();

	shader.Disable();
}
