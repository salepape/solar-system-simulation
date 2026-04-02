#include "DirectionalLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"



DirectionalLight::DirectionalLight(const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const bool inIsBlinn) :
	GLSLParams({ inDirection, inReflectionParams, inIsBlinn }), fubo("fubo_DirectionalLight", UniformShaderGroup::PROJECTION_VIEW)
{
	SetFUniforms();
}

void DirectionalLight::SetFUniforms()
{
	// Stored in a GLSL struct with in Fragment Shader
	UniformGLSLStruct uboStruct;
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.direction)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.reflectionParams.ambient)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.reflectionParams.diffuse)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.reflectionParams.specular)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.isBlinn), GLSLConstants::scalarSizeInBytes);
	fubo.SetData(uboStruct);
}

void DirectionalLight::SetLightDirectionFUniform(const glm::vec3& inDirection) const
{
	fubo.SetSubData(static_cast<const void*>(glm::value_ptr(inDirection)), GLSLConstants::vec4SizeInBytes, 0);
}