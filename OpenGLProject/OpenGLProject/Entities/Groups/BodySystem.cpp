#include "BodySystem.h"

#include <glm/geometric.hpp>

#include <cstdint>
#include <string>
#include <utility>

#include "Cameras/PerspectiveCamera.h"
#include "Entities/BodyRingsEntity.h"
#include "Rendering/Renderer.h"
#include "Rendering/ShaderLoader.h"



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

void BodySystem::Render(const bool isBillboard, PerspectiveCamera& camera, const glm::vec3& parentPosition, const float elapsedTime)
{
	celestialBody.Render(elapsedTime);

	orbit.Render(parentPosition);

	if (celestialBodyRings != nullptr)
	{
		celestialBodyRings->Render(celestialBody.GetModelMatrix());
	}

	if (isBillboard)
	{
		// Orient text billboards so the correct side (i.e. with the glyphs rendered in the correct direction) always faces the camera
		const glm::vec3& forward = glm::normalize(camera.GetPosition() - celestialBody.GetPosition());
		const glm::vec3& right = glm::cross(camera.GetUp(), forward);
		billboard.Render(celestialBody.GetPosition(), forward, right);
	}
}