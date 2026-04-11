#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/vec3.hpp>

#include "LightSourceComponent.h"
#include "UniformBuffer.h"



// Warning: make sure this struct always contain the same data as the mirrored one in GLSL
struct GLSLPointLightParams
{
	glm::vec3 position;

	ReflectionParams reflectionParams;
	AttenuationParams attenuationParams;

	bool isBlinn;
};

class PointLightComponent : public LightSourceComponent
{
public:
	PointLightComponent() = delete;
	PointLightComponent(const glm::vec3& inPosition, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const bool inIsBlinn = false);

	void SetPosition(const glm::vec3& inPosition) { GLSLParams.position = inPosition; }

	void SetFUniforms();
	void SetLightPositionFUniform(const glm::vec3& inPosition) const;

private:
	GLSLPointLightParams GLSLParams;
	UniformBuffer fubo;
};



#endif // POINT_LIGHT_H