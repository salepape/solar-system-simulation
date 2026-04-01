#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Belt.h"
#include "BodySystem.h"
#include "MilkyWay.h"
#include "Scene.h"
#include "Spacecraft.h"



class SolarSystem : public Scene
{
public:
	SolarSystem();

	void Update() override;

	const std::vector<BodySystem>& GetBodySystems() const { return bodySystems; }
	const std::vector<Belt>& GetBelts() const { return belts; }

	BodySystem& GetBodySystem(const std::string& inBodyName);

private:
	std::vector<BodySystem> bodySystems;
	std::vector<Belt> belts;

	// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun -> farthest celestial body'
	MilkyWay milkyWay;
	Spacecraft spacecraft;

	// @todo - Think about using a Builder Design Pattern to construct such class instances out of CSV files
	// Instantiate "spherical" celestial bodies/ring systems/belt systems, after loading data from .csv files,
	// and re-scaling it so we can visualise the whole Solar System without having to travel for too long
	void BuildBodySystems();
	void BuildBodySystemsLegend();
	void BuildBodyRings();
	void BuildBelts();
};



#endif // SOLAR_SYSTEM_H