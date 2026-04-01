#include "PointLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"



PointLight::PointLight(const glm::vec3& inPosition, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const bool inIsBlinn) : LightSource(inReflectionParams, inIsBlinn),
GLSLParams({ inPosition, inReflectionParams, inAttenuationParams, inIsBlinn }), fubo("fubo_PointLight", UniformShaderGroup::LINE_OF_SIGHT)
{
	SetFUniforms();
}

void PointLight::SetFUniforms()
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

// Called each frame when Headlight is turned on
void PointLight::SetLightPositionFUniform(const glm::vec3& inPosition) const
{
	fubo.SetSubData(static_cast<const void*>(glm::value_ptr(inPosition)), GLSLConstants::vec4SizeInBytes);
}