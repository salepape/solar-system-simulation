#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/vec3.hpp>

#include "LightSource.h"
#include "UniformBuffer.h"



class PointLight : public LightSource
{
public:
	PointLight();
	PointLight(const glm::vec3& inPosition, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const bool inIsBlinn);

private:
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	AttenuationParams attenuationParams;

	UniformBuffer& ubo;

	static bool isUBOFilled;



	void StoreDataInUBO();
};



#endif // POINT_LIGHT_H