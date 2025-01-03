#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include <glm/vec3.hpp>
#include <vector>



// Surface coefficient of reflection for each Phong component
struct ReflectionParams
{
	glm::vec3 ambient{ 0.0f };
	glm::vec3 diffuse{ 0.0f };
	glm::vec3 specular{ 0.0f };
};

// Terms of light intensity attenuation formula
struct AttenuationParams
{
	float constant{ 0.0f };
	float linear{ 0.0f };
	float quadratic{ 0.0f };
};

// Source casting rays of lights according to Phong Illumination model
class LightSource
{
public:
	LightSource(const ReflectionParams& inReflectionParams, const bool isBlinn = false);

	virtual void SetFUniforms() = 0;

protected:
	ReflectionParams reflectionParams;
	bool isBlinn{ false };
};



#endif // LIGHT_SOURCE_H