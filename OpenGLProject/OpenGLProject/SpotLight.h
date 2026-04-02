#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glm/vec3.hpp>

#include "LightSource.h"
#include "UniformBuffer.h"



struct SpotParams
{
	float cutoff{ 0.0f };
	float outerCutoff{ 0.0f };
};

// Warning: make sure this struct always contain the same data as the mirrored one in GLSL
struct GLSLSpotLightParams
{
	glm::vec3 position;
	glm::vec3 direction;

	ReflectionParams reflectionParams;
	AttenuationParams attenuationParams;
	SpotParams spotParams;

	bool isBlinn;
	bool isCameraFlashLight;
};

class SpotLight : public LightSource
{
public:
	SpotLight() = delete;
	SpotLight(const glm::vec3& inPosition, const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const SpotParams& inSpotParams, const bool inIsBlinn = false);

	void SetPosition(const glm::vec3& inPosition) { GLSLParams.position = inPosition; }
	void SetDirection(const glm::vec3& inDirection) { GLSLParams.direction = inDirection; }
	void SetActivationState(const bool isActive) { GLSLParams.isCameraFlashLight = isActive; }

	void SetFUniforms() override;
	void SetLightPositionFUniform(const glm::vec3& inPosition);
	void SetLightDirectionFUniform(const glm::vec3& inDirection);
	void SetIsCameraFlashLightFUniform(const bool isActive);

private:
	GLSLSpotLightParams GLSLParams;
	UniformBuffer fubo;
};



#endif // SPOT_LIGHT_H