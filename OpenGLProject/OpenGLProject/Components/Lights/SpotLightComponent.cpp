#include "SpotLightComponent.h"

#include <glm/gtc/type_ptr.hpp>

#include "Utils/Constants.h"



SpotLightComponent::SpotLightComponent(const glm::vec3& inPosition, const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const SpotParams& inSpotParams, const bool inIsBlinn) :
	GLSLParams({ inPosition, inDirection, inReflectionParams, inAttenuationParams, inSpotParams, inIsBlinn, false }), fubo("fubo_SpotLight", GLSLUniform::LINE_OF_SIGHT)
{
	SetFUniforms();
}

void SpotLightComponent::SetFUniforms()
{
	// Stored in a GLSL struct with in Fragment Shader
	UniformGLSLStruct uboStruct;
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.position)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.direction)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.reflectionParams.ambient)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.reflectionParams.diffuse)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(GLSLParams.reflectionParams.specular)), GLSLConstants::vec4SizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.attenuationParams.constant), GLSLConstants::scalarSizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.attenuationParams.linear), GLSLConstants::scalarSizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.attenuationParams.quadratic), GLSLConstants::scalarSizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.spotParams.cutoff), GLSLConstants::scalarSizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.spotParams.outerCutoff), GLSLConstants::scalarSizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.isBlinn), GLSLConstants::scalarSizeInBytes);
	uboStruct.AddUniformField(static_cast<const void*>(&GLSLParams.isCameraFlashLight), GLSLConstants::scalarSizeInBytes);
	fubo.SetData(uboStruct);
}

void SpotLightComponent::SetLightPositionFUniform(const glm::vec3& inPosition)
{
	SetPosition(inPosition);
	fubo.SetSubData(static_cast<const void*>(glm::value_ptr(inPosition)), GLSLConstants::vec4SizeInBytes, 0);
}

void SpotLightComponent::SetLightDirectionFUniform(const glm::vec3& inDirection)
{
	SetDirection(inDirection);
	fubo.SetSubData(static_cast<const void*>(glm::value_ptr(inDirection)), GLSLConstants::vec4SizeInBytes, GLSLConstants::vec4SizeInBytes);
}

void SpotLightComponent::SetIsCameraFlashLightFUniform(const bool isActive)
{
	SetActivationState(isActive);
	fubo.SetSubData(static_cast<const void*>(&isActive), GLSLConstants::scalarSizeInBytes, 5 * GLSLConstants::vec4SizeInBytes + 6 * GLSLConstants::scalarSizeInBytes);
}