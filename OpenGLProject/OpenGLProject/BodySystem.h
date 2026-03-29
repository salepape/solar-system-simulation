#ifndef BODY_SYSTEM_H
#define BODY_SYSTEM_H

#include <glm/vec3.hpp>
#include <memory>

#include "Billboard.h"
#include "BodyRings.h"
#include "CelestialBody.h"
#include "LightSource.h"
#include "Orbit.h"

class PerspectiveCamera;
class Renderer;
class TextRenderer;



// Represent a set of scene entities: the celestial body, its orbit, its ring system, and its name on a billboard
class BodySystem
{
public:
	BodySystem() = delete;
	BodySystem(BodyData&& inBodyData);

	BodySystem(const BodySystem& inBodySystem) = default;
	BodySystem& operator = (const BodySystem& inBodySystem) = delete;

	BodySystem(BodySystem&& inBodySystem) = default;
	BodySystem& operator = (BodySystem&& inBodySystem) = delete;

	virtual ~BodySystem() = default;

	CelestialBody celestialBody;
	Orbit orbit;
	Billboard billboard;

	// Will be filled once the Rings have been parsed from the CSV file, hence not in the constructor of this class
	std::unique_ptr<BodyRings> celestialBodyRings;

	void Render(const Renderer& renderer, const bool isBillboard, TextRenderer& textRenderer, PerspectiveCamera& camera, const glm::vec3& parentPosition, const float elapsedTime);

private:
	// @todo - Would be an idea to move this to a proper Sun class: Planets do not glow on visible spectrum (tiny bit for Earth)
	std::unique_ptr<LightSource> lightSource;
};



#endif // BODY_SYSTEM_H