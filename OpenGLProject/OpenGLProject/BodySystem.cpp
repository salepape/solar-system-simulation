#include "BodySystem.h"

#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <string>

#include "BodyRings.h"
#include "LightSource.h"
#include "PointLight.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Utils.h"



BodySystem::BodySystem(BodyData&& inBodyData) :
	celestialBody(std::move(inBodyData)),
	orbit(inBodyData.texturePath, inBodyData.distanceToParent),
	billboard(ResourceLoader::GetNameFromTexturePath(inBodyData.texturePath)),
	preComputations(LoadPreComputations(inBodyData))
{
	const std::string& bodyName = celestialBody.GetName();
	if (bodyName == "Sun")
	{
		// Set up the lighting for all scene entities according to Sun position/light emission parameters
		lightSource = std::make_unique<PointLight>(celestialBody.GetPosition(),
			ReflectionParams{ glm::vec3(0.25f), glm::vec3(0.95f), glm::vec3(1.0f) },
			AttenuationParams{ 1.0f, 0.00045f, 0.00000075f });
	}

	// Attach a ring system to the celestial body if one
	if (inBodyData.hasRings)
	{
		bodyRings = std::make_unique<BodyRings>(std::move(ResourceLoader::GetBodyRings(bodyName)));
	}
}

PreComputations BodySystem::LoadPreComputations(const BodyData& inBodyData)
{
	const float orbitalInclinationInRad = glm::radians(inBodyData.orbitalInclination);

	float textHeightFactor = 1.5f;
	float textScaleFactor = 0.01f;
	// If the current celestial body is a satellite (i.e. has a parent)
	if (inBodyData.parentID != -1)
	{
		textHeightFactor = 3.5f;
		textScaleFactor = 0.03f;
	}

	return {
		inBodyData.orbitalPeriod == 0.0f ? 0.0f : Utils::doublePi * 1.0f / inBodyData.orbitalPeriod,
		inBodyData.spinPeriod == 0.0f ? 0.0f : Utils::doublePi * 1.0f / inBodyData.spinPeriod,

		glm::radians(inBodyData.obliquity),
		orbitalInclinationInRad,

		inBodyData.distanceToParent * glm::cos(orbitalInclinationInRad),
		inBodyData.distanceToParent * glm::sin(orbitalInclinationInRad),

		inBodyData.radius * textHeightFactor,
		inBodyData.radius * textScaleFactor
	};
}

void BodySystem::Render(const Renderer& renderer, const float /*elapsedTime*/)
{
	celestialBody.Render(renderer);

	orbit.Render(renderer);

	if (bodyRings)
	{
		bodyRings->Render(renderer);
	}
}