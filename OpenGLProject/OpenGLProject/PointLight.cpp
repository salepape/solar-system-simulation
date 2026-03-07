#include "PointLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "ResourceLoader.h"
#include "UniformBuffer.h"



PointLight::PointLight(const glm::vec3& inPosition, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const bool inIsBlinn) : LightSource(inReflectionParams, inIsBlinn),
position(inPosition), attenuationParams(inAttenuationParams), ubo(ResourceLoader::GetUBO("ubo_PointLight"))
{
	SetFUniforms();
}

void PointLight::SetFUniforms()
{
	ubo.SetSubData({
		{ static_cast<const void*>(glm::value_ptr(position)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.ambient)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.diffuse)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.specular)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(&attenuationParams.constant), GLSLConstants::scalarSizeInBytes },
		{ static_cast<const void*>(&attenuationParams.linear), GLSLConstants::scalarSizeInBytes },
		{ static_cast<const void*>(&attenuationParams.quadratic), GLSLConstants::scalarSizeInBytes },
		{ static_cast<const void*>(&isBlinn), GLSLConstants::scalarSizeInBytes }
		});
}

void PointLight::SetLightPositionFUniform(const glm::vec3& inPosition)
{
	ubo.SetSubData(static_cast<const void*>(glm::value_ptr(inPosition)), GLSLConstants::vec4SizeInBytes);
}