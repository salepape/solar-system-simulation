#include "DirectionalLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "ResourceLoader.h"
#include "UniformBuffer.h"



DirectionalLight::DirectionalLight(const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const bool inIsBlinn) : LightSource(inReflectionParams, inIsBlinn),
direction(inDirection), ubo(ResourceLoader::GetUBO("ubo_DirectionalLight"))
{
	SetFUniforms();
}

void DirectionalLight::SetFUniforms()
{
	ubo.SetSubData({
		{ static_cast<const void*>(glm::value_ptr(direction)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.ambient)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.diffuse)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.specular)), GLSLConstants::vec4SizeInBytes },
		{ static_cast<const void*>(&isBlinn), GLSLConstants::scalarSizeInBytes }
		});
}

void DirectionalLight::SetLightDirectionFUniform(const glm::vec3& inDirection)
{
	ubo.SetSubData(static_cast<const void*>(glm::value_ptr(inDirection)), GLSLConstants::vec4SizeInBytes);
}