#ifndef BODY_SYSTEM_H
#define BODY_SYSTEM_H

#include <glm/vec3.hpp>
#include <memory>

#include "Billboard.h"
#include "CelestialBody.h"
#include "Orbit.h"

class BodyRings;
class LightSource;
class PerspectiveCamera;
class Renderer;
struct RingsData;
class TextRenderer;



// Represent a set of scene entities: the celestial body, its orbit, its ring system, and its name on a billboard
class BodySystem
{
public:
	// Default constructor (not needed)
	BodySystem() = delete;

	// User-defined constructor (to be used when building a BodySystem instance in-place)
	BodySystem(BodyData&& inBodyData);

	// Copy constructor (needed when we call GetBodySystem(), as it gets a shallow copy of an instance of this class)
	BodySystem(const BodySystem& inBodySystem) = default;
	const BodySystem& operator = (const BodySystem& inBodySystem) = delete;

	// Move constructor (needed when building a BodySystem in-place)
	BodySystem(BodySystem&& inBodySystem) = default;
	BodySystem&& operator = (BodySystem&& inBodySystem) = delete;

	// Destructor (not virtual needed, until child classes exist)
	~BodySystem() = default;

	CelestialBody& GetCelestialBody() { return celestialBody; }
	const CelestialBody& GetCelestialBody() const { return celestialBody; }

	void SetBodyRings(RingsData&& inRingsData);

	void Render(const Renderer& renderer, const bool isBillboard, TextRenderer& textRenderer, PerspectiveCamera& camera, const glm::vec3& parentPosition, const float elapsedTime);

private:
	CelestialBody celestialBody;
	Orbit orbit;
	Billboard billboard;

	// Will be filled once the Rings have been parsed from the CSV file, hence not in the constructor of this class
	std::shared_ptr<BodyRings> celestialBodyRings;

	// @todo - Would be an idea to move this to a proper Sun class: Planets do not glow on visible spectrum (tiny bit for Earth)
	std::shared_ptr<LightSource> lightSource;
};



#endif // BODY_SYSTEM_H