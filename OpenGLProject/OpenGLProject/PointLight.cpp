#include "PointLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "ResourceLoader.h"
#include "UniformBuffer.h"
#include "Utils.h"

bool PointLight::isUBOFilled = false;



PointLight::PointLight() : LightSource({ glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.95f, 0.95f, 0.95f), glm::vec3(1.0f, 1.0f, 1.0f) }),
ubo(ResourceLoader::GetUBO("ubo_PointLightParams"))
{
	StoreDataInUBO();
}

PointLight::PointLight(const glm::vec3& inPosition, const ReflectionParams& inReflectionParams, const AttenuationParams& inAttenuationParams, const bool inIsBlinn) : LightSource(inReflectionParams, inIsBlinn),
position(inPosition), attenuationParams(inAttenuationParams), ubo(ResourceLoader::GetUBO("ubo_PointLightParams"))
{
	StoreDataInUBO();
}

void PointLight::StoreDataInUBO()
{
	// No need to fill the common UBO by the same values each time a PointLight is created (and values are constants in the Render loop)
	if (PointLight::isUBOFilled)
	{
		return;
	}

	ubo.InitSubData({
		{ static_cast<const void*>(glm::value_ptr(position)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.ambient)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.diffuse)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(reflectionParams.specular)), Utils::vec4Size },
		{ static_cast<const void*>(&attenuationParams.constant), Utils::scalarSize },
		{ static_cast<const void*>(&attenuationParams.linear), Utils::scalarSize },
		{ static_cast<const void*>(&attenuationParams.quadratic), Utils::scalarSize },
		{ static_cast<const void*>(&isBlinn), Utils::scalarSize }
		});

	PointLight::isUBOFilled = true;
}