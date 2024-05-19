#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/vec3.hpp>
#include <vector>

#include "LightSource.h"



class DirectionalLight : public LightSource
{
public:
	DirectionalLight();
	DirectionalLight(const glm::vec3& inDirection, const ReflectionParams& inReflectionParams, const bool inIsBlinn);

	void Store(const std::vector<uint32_t>& entitiesShadersIDs) override;

private:
	glm::vec3 direction{ -0.2f, -1.0f, -0.3f };
};



#endif // DIRECTIONAL_LIGHT_H