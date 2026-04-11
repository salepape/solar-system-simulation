#include "BodySystem.h"

#include <cstdint>
#include <glm/geometric.hpp>
#include <string>
#include <utility>

#include "BodyRingsEntity.h"
#include "PerspectiveCamera.h"
#include "Renderer.h"
#include "ShaderLoader.h"



BodySystem::BodySystem(BodyData&& inBodyData) :
	celestialBody(std::move(inBodyData)),
	orbit(celestialBody.GetBodyData()),
	billboard(celestialBody.GetBodyData())
{

}

void BodySystem::SetBodyRings(RingsData&& inRingsData)
{
	celestialBodyRings = std::make_shared<BodyRingsEntity, RingsData&&>(std::move(inRingsData));
}

void BodySystem::Render(const Renderer& renderer, const bool isBillboard, TextRenderer& textRenderer, PerspectiveCamera& camera, const glm::vec3& parentPosition, const float elapsedTime)
{
	celestialBody.Render(renderer, elapsedTime);

	orbit.Render(renderer, parentPosition, elapsedTime);

	if (celestialBodyRings != nullptr)
	{
		celestialBodyRings->Render(renderer, celestialBody.GetModelMatrix(), elapsedTime);
	}

	if (isBillboard)
	{
		// Orient text billboards so the correct side (i.e. with the glyphs rendered in the correct direction) always faces the camera
		const glm::vec3& forward = glm::normalize(camera.GetPosition() - celestialBody.GetPosition());
		const glm::vec3& right = glm::cross(camera.GetUp(), forward);
		billboard.Render(renderer, textRenderer, celestialBody.GetPosition(), forward, right);
	}
}