#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "UniformBuffer.h"



// Surface coefficient of reflection for each Phong component
struct ReflectionParams
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

// Terms of light intensity attenuation formula
struct AttenuationParams
{
	float constant{ 1.0f };
	float linear{ 0.0007f };
	float quadratic{ 0.000002f };
};

// Source casting rays of lights according to Phong Illumination model
class LightSource
{
public:
	LightSource(const ReflectionParams& inReflectionParams, const bool isBlinn = false);
	virtual void Store(const std::vector<uint32_t>& entitiesShadersIDs) { };

protected:
	const ReflectionParams reflectionParams;

	bool isBlinn{ false };

	std::unique_ptr<UniformBuffer> ubo;
};



#endif // LIGHT_SOURCE_H