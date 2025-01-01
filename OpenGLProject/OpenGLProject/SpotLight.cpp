#include "SpotLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "ResourceLoader.h"
#include "UniformBuffer.h"
#include "Utils.h"



SpotLight::SpotLight(const glm::vec3& inPosition, const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const SpotParams& inSpotParams, const bool inIsBlinn) : LightSource(inReflectionParams),
position(inPosition), direction(inDirection), attenuationParams(inAttenuationParams), spotParams(inSpotParams), ubo(ResourceLoader::GetUBO("ubo_SpotLight"))
{
	SetFUniforms();
}

void SpotLight::SetFUniforms()
{
	ubo.SetSubData({
		{ static_cast<const void*>(glm::value_ptr(position)), Utils::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(direction)), Utils::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.ambient)), Utils::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.diffuse)), Utils::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.specular)), Utils::vec4SizeInBytes },
		{ static_cast<const void*>(&attenuationParams.constant), Utils::scalarSizeInBytes },
		{ static_cast<const void*>(&attenuationParams.linear), Utils::scalarSizeInBytes },
		{ static_cast<const void*>(&attenuationParams.quadratic), Utils::scalarSizeInBytes },
		{ static_cast<const void*>(&spotParams.cutoff), Utils::scalarSizeInBytes },
		{ static_cast<const void*>(&spotParams.outerCutoff), Utils::scalarSizeInBytes },
		{ static_cast<const void*>(&isBlinn), Utils::scalarSizeInBytes },
		{ static_cast<const void*>(&isCameraFlashLight), Utils::scalarSizeInBytes }
		});
}

void SpotLight::SetLightPositionFUniform(const glm::vec3& inPosition)
{
	SetPosition(inPosition);
	ubo.SetSubData(static_cast<const void*>(glm::value_ptr(inPosition)), Utils::vec4SizeInBytes);
}

void SpotLight::SetLightDirectionFUniform(const glm::vec3& inDirection)
{
	SetDirection(inDirection);
	ubo.SetSubData(static_cast<const void*>(glm::value_ptr(inDirection)), Utils::vec4SizeInBytes, Utils::vec4SizeInBytes);
}

void SpotLight::SetIsCameraFlashLightFUniform(const bool isActive)
{
	SetActivationState(isActive);
	ubo.SetSubData(static_cast<const void*>(&isActive), Utils::scalarSizeInBytes, 5 * Utils::vec4SizeInBytes + 6 * Utils::scalarSizeInBytes);
}