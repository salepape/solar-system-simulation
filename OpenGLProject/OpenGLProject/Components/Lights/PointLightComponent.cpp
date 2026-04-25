#include "PointLightComponent.h"

#include <glm/gtc/type_ptr.hpp>

#include "Utils/Constants.h"



PointLightComponent::PointLightComponent(const glm::vec3& inPosition, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const bool inIsBlinn) :
	GLSLParams({ inPosition, inReflectionParams, inAttenuationParams, inIsBlinn }), fubo("fubo_PointLight", GLSLUniform::LINE_OF_SIGHT)
{
	SetFUniforms();
}

void PointLightComponent::SetFUniforms()
{
	// Stored in a GLSL struct with in Fragment Shader
	UniformGLSLStruct uboStruct;
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.position)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.reflectionParams.ambient)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.reflectionParams.diffuse)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.reflectionParams.specular)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.attenuationParams.constant), GLSLConstants::scalarSizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.attenuationParams.linear), GLSLConstants::scalarSizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.attenuationParams.quadratic), GLSLConstants::scalarSizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.isBlinn), GLSLConstants::scalarSizeInBytes);
	fubo.SetData(uboStruct);
}

void PointLightComponent::SetLightPositionFUniform(const glm::vec3& inPosition) const
{
	fubo.SetSubData(static_cast<const void*>(glm::value_ptr(inPosition)), GLSLConstants::vec4SizeInBytes, 0);
}