#include "DirectionalLight.h"

#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "UniformBuffer.h"
#include "Utils.h"



DirectionalLight::DirectionalLight() :
	LightSource({ {0.75f, 0.75f, 0.75f}, {0.5f, 0.5f, 0.5f}, { 0.95f, 0.95f, 0.95f } })
{

}

DirectionalLight::DirectionalLight(const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const bool inIsBlinn) :
	direction(inDirection), LightSource(inReflectionParams, inIsBlinn)
{

}

void DirectionalLight::Store(const std::vector<uint32_t>& entitiesShadersIDs)
{
	ubo = std::make_unique<UniformBuffer>(entitiesShadersIDs, "directionalLightParams", 4 * Utils::vec4Size + Utils::scalarSize);
	ubo->InitSubData({
		{ static_cast<const void*>(glm::value_ptr(direction)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.ambient)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.diffuse)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.specular)), Utils::vec4Size },
		{ static_cast<const void*>(&isBlinn), Utils::scalarSize }
		});
}