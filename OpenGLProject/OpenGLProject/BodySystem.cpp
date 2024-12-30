#include "BodySystem.h"

#include <glm/vec3.hpp>
#include <string>

#include "BodyRings.h"
#include "LightSource.h"
#include "PointLight.h"
#include "Renderer.h"
#include "ResourceLoader.h"



BodySystem::BodySystem(BodyData&& inBodyData) :
	celestialBody(std::move(inBodyData)),
	orbit(std::move(inBodyData)),
	billboard(std::move(inBodyData))
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

void BodySystem::Render(const Renderer& renderer, const float /*elapsedTime*/)
{
	celestialBody.Render(renderer);

	orbit.Render(renderer);

	if (bodyRings)
	{
		bodyRings->Render(renderer);
	}
}