#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glm/vec3.hpp>

#include "LightSource.h"

class UniformBuffer;



struct SpotParams
{
	float cutoff{ 0.0f };
	float outerCutoff{ 0.0f };
};

class SpotLight : public LightSource
{
public:
	SpotLight() = delete;
	SpotLight(const glm::vec3& inPosition, const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const SpotParams& inSpotParams, const bool inIsBlinn = false);

	void SetPosition(const glm::vec3& inPosition) { position = inPosition; }
	void SetDirection(const glm::vec3 inDirection) { direction = inDirection; }
	void SetActivationState(const bool isActive) { isCameraFlashLight = isActive; }

	void SetFUniforms() override;
	void SetLightPositionFUniform(const glm::vec3& inPosition);
	void SetLightDirectionFUniform(const glm::vec3& inDirection);
	void SetIsCameraFlashLightFUniform(const bool isActive);

private:
	glm::vec3 position{ 0.0f };
	glm::vec3 direction{ 0.0f };
	AttenuationParams attenuationParams;
	SpotParams spotParams;
	bool isCameraFlashLight{ false };

	UniformBuffer& ubo;
};



#endif // SPOT_LIGHT_H