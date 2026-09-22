#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include <glm/vec3.hpp>



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

// @todo - Really useful: do not contain anything to justify this class as abstract...
// Source casting rays of lights according to Phong Illumination model
class LightSourceComponent
{
public:
	// Virtual destructor (needed, as class is not final)
	virtual ~LightSourceComponent() = default;
};



#endif // LIGHT_SOURCE_H
