#ifndef BODY_SYSTEM_H
#define BODY_SYSTEM_H

#include <glm/vec3.hpp>
#include <memory>

#include "BillboardEntity.h"
#include "CelestialBodyEntity.h"
#include "OrbitEntity.h"

class BodyRingsEntity;
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

	CelestialBodyEntity& GetCelestialBody() { return celestialBody; }
	const CelestialBodyEntity& GetCelestialBody() const { return celestialBody; }

	void SetBodyRings(RingsData&& inRingsData);

	void Render(const Renderer& renderer, const bool isBillboard, TextRenderer& textRenderer, PerspectiveCamera& camera, const glm::vec3& parentPosition, const float elapsedTime);

private:
	CelestialBodyEntity celestialBody;
	OrbitEntity orbit;
	BillboardEntity billboard;

	// Will be filled once the Rings have been parsed from the CSV file, hence not in the constructor of this class
	std::shared_ptr<BodyRingsEntity> celestialBodyRings;
};



#endif // BODY_SYSTEM_H