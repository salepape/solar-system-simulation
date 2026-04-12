#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>

#include "OpenGLProject/Entities/BeltEntity.h"
#include "OpenGLProject/Entities/MilkyWayEntity.h"
#include "OpenGLProject/Entities/SpacecraftEntity.h"
#include "OpenGLProject/Entities/Groups/BodySystem.h"
#include "OpenGLProject/Scene.h"



class SolarSystem : public Scene
{
public:
	SolarSystem();

	void Update() override;

	const std::vector<BodySystem>& GetBodySystems() const { return bodySystems; }
	const std::vector<BeltEntity>& GetBelts() const { return belts; }

	BodySystem& GetBodySystem(const std::string& inBodyName);

private:
	// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun -> farthest celestial body'
	MilkyWayEntity milkyWay;
	SpacecraftEntity spacecraft;

	std::vector<BodySystem> bodySystems;
	std::vector<BeltEntity> belts;

	// @todo - Think about using a Builder Design Pattern to construct such class instances out of CSV files
	// Instantiate "spherical" celestial bodies/ring systems/belt systems, after loading data from .csv files,
	// and re-scaling it so we can visualise the whole Solar System without having to travel for too long
	void BuildBodySystems();
	void BuildBodySystemsLegend();
	void BuildBodyRings();
	void BuildBelts();
};



#endif // SOLAR_SYSTEM_H