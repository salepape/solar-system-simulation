#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include <glm/vec3.hpp>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Scene/Scene.h"



// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun -> farthest celestial body'
class SolarSystem : public Scene
{
public:
	SolarSystem();

	void Update(const float deltaTime, const std::optional<SceneEntityHandle> /*handle*/) override;

private:
	// @todo - Think about using a Builder Design Pattern to construct such class instances out of CSV files
	// Instantiate "spherical" celestial bodies/ring systems/belt systems, after loading data from .csv files,
	// and re-scaling it so we can visualise the whole Solar System without having to travel for too long
	void BuildBackground();
	void BuildBodySystems();
	void BuildBodyRings();
	void BuildBelts();

	class CelestialBodyEntity& GetBody(const std::string& bodyName) const;
};



#endif // SOLAR_SYSTEM_H
