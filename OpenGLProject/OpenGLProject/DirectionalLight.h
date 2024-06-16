#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/vec3.hpp>

#include "LightSource.h"

class Shader;



class DirectionalLight : public LightSource
{
public:
	DirectionalLight() = delete;
	DirectionalLight(Shader& inShader, const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const bool inIsBlinn = false);

private:
	Shader& shader;
	glm::vec3 direction{ 0.0f };



	void SetFUniforms() override;
};



#endif // DIRECTIONAL_LIGHT_H