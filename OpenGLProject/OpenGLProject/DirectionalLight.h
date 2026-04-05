#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/vec3.hpp>

#include "LightSource.h"
#include "UniformBuffer.h"



// Warning: make sure this struct always contain the same data as the mirrored one in GLSL
struct GLSLDirectionalLightParams
{
	glm::vec3 direction;

	ReflectionParams reflectionParams;

	bool isBlinn;
};

class DirectionalLight : public LightSource
{
public:
	DirectionalLight() = delete;
	DirectionalLight(const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const bool inIsBlinn = false);

	void SetLightDirectionFUniform(const glm::vec3& inDirection) const;

private:
	GLSLDirectionalLightParams GLSLParams;
	UniformBuffer fubo;

	void SetFUniforms();
};



#endif // DIRECTIONAL_LIGHT_H