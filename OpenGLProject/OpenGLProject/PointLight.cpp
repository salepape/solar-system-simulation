#include "PointLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "ResourceLoader.h"
#include "UniformBuffer.h"
#include "Utils.h"



PointLight::PointLight(const glm::vec3& inPosition, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const bool inIsBlinn) : LightSource(inReflectionParams, inIsBlinn),
position(inPosition), attenuationParams(inAttenuationParams), ubo(ResourceLoader::GetUBO("ubo_PointLight"))
{
	SetFUniforms();
}

void PointLight::SetFUniforms()
{
	ubo.InitSubData({
		{ static_cast<const void*>(glm::value_ptr(position)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.ambient)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.diffuse)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.specular)), Utils::vec4Size },
		{ static_cast<const void*>(&attenuationParams.constant), Utils::scalarSize },
		{ static_cast<const void*>(&attenuationParams.linear), Utils::scalarSize },
		{ static_cast<const void*>(&attenuationParams.quadratic), Utils::scalarSize },
		{ static_cast<const void*>(&isBlinn), Utils::scalarSize }
		});
}

void PointLight::SetLightPositionFUniform(const glm::vec3& inPosition)
{
	ubo.InitSubData({ { static_cast<const void*>(glm::value_ptr(position)), Utils::vec4Size } });
}