#include "DirectionalLight.h"

#include "Shader.h"



DirectionalLight::DirectionalLight(Shader& inShader, const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const bool inIsBlinn) : LightSource(inReflectionParams, inIsBlinn),
shader(inShader), direction(inDirection)
{
	SetFUniforms();
}

void DirectionalLight::SetFUniforms()
{
	shader.Enable();
	shader.setUniformVec3("light.fu_Dir", direction);
	shader.setUniformVec3("light.fu_AmbientReflectCoef", reflectionParams.ambient);
	shader.setUniformVec3("light.fu_DiffuseReflectCoef", reflectionParams.diffuse);
	shader.setUniformVec3("light.fu_SpecularReflectCoef", reflectionParams.specular);
	shader.setUniformBool("light.fu_IsBlinn", isBlinn);
	shader.Disable();
}