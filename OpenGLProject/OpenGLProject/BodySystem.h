#ifndef BODY_SYSTEM_H
#define BODY_SYSTEM_H

#include <memory>

#include "Billboard.h"
#include "CelestialBody.h"
#include "Orbit.h"

class LightSource;
class BodyRings;
class Renderer;



// Represent a set of scene entities: the celestial body, its orbit, its ring system, and its name on a billboard
class BodySystem
{
public:
	BodySystem() = delete;
	BodySystem(BodyData&& inBodyData);

	CelestialBody celestialBody;
	Orbit orbit;
	Billboard billboard;

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f);

private:
	std::unique_ptr<LightSource> lightSource;
	std::unique_ptr<BodyRings> bodyRings;
};



#endif // BODY_SYSTEM_H