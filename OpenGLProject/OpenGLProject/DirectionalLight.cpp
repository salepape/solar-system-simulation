#include "DirectionalLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "ResourceLoader.h"
#include "UniformBuffer.h"
#include "Utils.h"



DirectionalLight::DirectionalLight(const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const bool inIsBlinn) : LightSource(inReflectionParams, inIsBlinn),
direction(inDirection), ubo(ResourceLoader::GetUBO("ubo_DirectionalLight"))
{
	SetFUniforms();
}

void DirectionalLight::SetFUniforms()
{
	ubo.InitSubData({
		{ static_cast<const void*>(glm::value_ptr(direction)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.ambient)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.diffuse)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.specular)), Utils::vec4Size },
		{ static_cast<const void*>(&isBlinn), Utils::scalarSize }
		});
}

void DirectionalLight::SetLightDirectionFUniform(const glm::vec3& inDirection)
{
	ubo.InitSubData({ { static_cast<const void*>(glm::value_ptr(inDirection)), Utils::vec4Size } });
}