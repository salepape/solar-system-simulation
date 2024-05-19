#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/vec3.hpp>
#include <vector>

#include "LightSource.h"



class PointLight : public LightSource
{
public:
	PointLight();
	PointLight(const glm::vec3& inPosition, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const bool inIsBlinn);

	void Store(const std::vector<uint32_t>& entitiesShadersIDs) override;

private:
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };

	AttenuationParams attenuationParams;
};



#endif // POINT_LIGHT_H