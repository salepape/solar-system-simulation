#ifndef BODY_SYSTEM_H
#define BODY_SYSTEM_H

#include <memory>

#include "Billboard.h"
#include "CelestialBody.h"
#include "Material.h"
#include "Orbit.h"

class LightSource;
class BodyRings;
class Renderer;



// Represent all Render loop constants of a celestial body
struct PreComputations
{
	float orbitAngularFreq{ 0.0f };			// Angular frequency for orbital motion [in radians/Earth days]
	float spinAngularFreq{ 0.0f };			// Angular frequency for spin motion [in radians/Earth days]

	float obliquityInRad{ 0.0f };			// Obliquity converted [in radians]
	float orbInclinationInRad{ 0.0f };		// Orbital Inclination converted [in radians]

	float distCosOrbInclination{ 0.0f };
	float distSinOrbInclination{ 0.0f };

	float textHeight{ 0.0f };
	float textScale{ 0.0f };
};

// Represent a set of scene entities: the celestial body, its orbit, its ring system, and its name on a billboard
class BodySystem
{
public:
	BodySystem() = delete;
	BodySystem(BodyData&& inBodyData);

	CelestialBody celestialBody;
	Orbit orbit;
	Billboard billboard;

	const PreComputations& GetPreComputations() const { return preComputations; }

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f);

private:
	PreComputations preComputations;
	std::unique_ptr<LightSource> lightSource;
	std::unique_ptr<BodyRings> bodyRings;

	PreComputations LoadPreComputations(const BodyData& inBodyData);
};



#endif // BODY_SYSTEM_H