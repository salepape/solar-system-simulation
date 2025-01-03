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
	ubo.SetSubData({
		{ static_cast<const void*>(glm::value_ptr(direction)), Utils::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.ambient)), Utils::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.diffuse)), Utils::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.specular)), Utils::vec4SizeInBytes },
		{ static_cast<const void*>(&isBlinn), Utils::scalarSizeInBytes }
		});
}

void DirectionalLight::SetLightDirectionFUniform(const glm::vec3& inDirection)
{
	ubo.SetSubData(static_cast<const void*>(glm::value_ptr(inDirection)), Utils::vec4SizeInBytes);
}