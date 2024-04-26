#include "PointLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "UniformBuffer.h"
#include "Utils.h"



PointLight::PointLight() :
	LightSource({ { 0.2f, 0.2f, 0.2f }, { 0.95f, 0.95f, 0.95f }, { 1.0f, 1.0f, 1.0f } })
{

}

PointLight::PointLight(const glm::vec3& inPosition, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const bool inIsBlinn) :
	position(inPosition), attenuationParams(inAttenuationParams), LightSource(inReflectionParams, inIsBlinn)
{

}

void PointLight::Store(const std::vector<unsigned int>& entitiesShadersIDs)
{
	ubo = new UniformBuffer(entitiesShadersIDs, "pointLightParams", static_cast<size_t>(4 * Utils::vec4Size + 4 * Utils::scalarSize));
	ubo->InitSubData({
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