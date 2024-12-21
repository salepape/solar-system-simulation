#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include <vector>

#include "Belt.h"
#include "CelestialBody.h"
#include "MilkyWay.h"
#include "Scene.h"



// All member containers of this class are filled using the ResourceLoader
class SolarSystem : public Scene
{
public:
	SolarSystem();

	void Update() override;

	static std::vector<CelestialBody>& GetCelestialBodiesVector();
	static std::vector<Belt>& GetBeltsVector();

private:
	// Container for all planets, dwarf planets, and moons
	static std::vector<CelestialBody> celestialBodies;

	// Container for all belts composed of small bodies
	static std::vector<Belt> belts;

	MilkyWay milkyWay;
};



#endif // SOLAR_SYSTEM_H