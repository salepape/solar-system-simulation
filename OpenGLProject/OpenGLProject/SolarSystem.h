#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include <vector>

#include "Belt.h"
#include "CelestialBody.h"
#include "MilkyWay.h"
#include "Scene.h"



class SolarSystem : public Scene
{
public:
	SolarSystem();

	void Update() override;

	static std::vector<CelestialBody>& GetCelestialBodiesVector();
	static std::vector<Belt>& GetBeltsVector();

private:
	// Container for all planets, moons and dward planets (filled using the ResourceLoader)
	static std::vector<CelestialBody> celestialBodies;

	// Container for Asteroid and Kuiper belts (filled using the ResourceLoader)
	static std::vector<Belt> belts;

	MilkyWay milkyWay;
};



#endif // SOLAR_SYSTEM_H