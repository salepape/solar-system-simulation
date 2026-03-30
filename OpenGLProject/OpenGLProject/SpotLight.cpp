#include "SpotLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "ResourceLoader.h"
#include "UniformBuffer.h"



SpotLight::SpotLight(const glm::vec3& inPosition, const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const SpotParams& inSpotParams, const bool inIsBlinn) : LightSource(inReflectionParams),
position(inPosition), direction(inDirection), attenuationParams(inAttenuationParams), spotParams(inSpotParams), ubo(ShaderLoader::GetUBO("ubo_SpotLight"))
{
	SetFUniforms();
}

void SpotLight::SetFUniforms()
{
	ubo.SetSubData({
		{ static_cast<const void*>(glm::value_ptr(position)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(direction)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.ambient)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.diffuse)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.specular)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(&attenuationParams.constant), GLSLConstants::scalarSizeInBytes },
		{ static_cast<const void*>(&attenuationParams.linear), GLSLConstants::scalarSizeInBytes },
		{ static_cast<const void*>(&attenuationParams.quadratic), GLSLConstants::scalarSizeInBytes },
		{ static_cast<const void*>(&spotParams.cutoff), GLSLConstants::scalarSizeInBytes },
		{ static_cast<const void*>(&spotParams.outerCutoff), GLSLConstants::scalarSizeInBytes },
		{ static_cast<const void*>(&isBlinn), GLSLConstants::scalarSizeInBytes },
		{ static_cast<const void*>(&isCameraFlashLight), GLSLConstants::scalarSizeInBytes }
		});
}

void SpotLight::SetLightPositionFUniform(const glm::vec3& inPosition)
{
	SetPosition(inPosition);
	ubo.SetSubData(static_cast<const void*>(glm::value_ptr(inPosition)), GLSLConstants::vec4SizeInBytes);
}

void SpotLight::SetLightDirectionFUniform(const glm::vec3& inDirection)
{
	SetDirection(inDirection);
	ubo.SetSubData(static_cast<const void*>(glm::value_ptr(inDirection)), GLSLConstants::vec4SizeInBytes, GLSLConstants::vec4SizeInBytes);
}

void SpotLight::SetIsCameraFlashLightFUniform(const bool isActive)
{
	SetActivationState(isActive);
	ubo.SetSubData(static_cast<const void*>(&isActive), GLSLConstants::scalarSizeInBytes, 5 * GLSLConstants::vec4SizeInBytes + 6 * GLSLConstants::scalarSizeInBytes);
}