#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/vec3.hpp>

#include "LightSource.h"

class UniformBuffer;



class DirectionalLight : public LightSource
{
public:
	DirectionalLight() = delete;
	DirectionalLight(const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const bool inIsBlinn = false);

	void SetLightDirectionFUniform(const glm::vec3& inDirection);

private:
	glm::vec3 direction{ 0.0f };

	UniformBuffer& ubo;


	void SetFUniforms() override;
};



#endif // DIRECTIONAL_LIGHT_H