#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/vec3.hpp>

#include "LightSource.h"

class Shader;



class DirectionalLight : public LightSource
{
public:
	DirectionalLight(Shader& inShader);
	DirectionalLight(Shader& inShader, const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const bool inIsBlinn);

private:
	Shader& shader;
	glm::vec3 direction{ -0.2f, -1.0f, -0.3f };

	void SetFUniforms();
};



#endif // DIRECTIONAL_LIGHT_H