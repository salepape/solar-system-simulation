#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/vec3.hpp>

#include "LightSource.h"
#include "UniformBuffer.h"



class PointLight : public LightSource
{
public:
	PointLight() = delete;
	PointLight(const glm::vec3& inPosition, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const bool inIsBlinn = false);
	
	void SetPosition(const glm::vec3& inPosition) { position = inPosition; }

	void SetFUniforms() override;
	void SetLightPositionFUniform(const glm::vec3& inPosition);

private:
	glm::vec3 position{ 0.0f };
	AttenuationParams attenuationParams;

	UniformBuffer& ubo;
};



#endif // POINT_LIGHT_H