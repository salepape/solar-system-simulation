#ifndef SOLAR_SYSTEM_SIMULATION_H
#define SOLAR_SYSTEM_SIMULATION_H

#include <memory>
#include <string>

#include "Application/Application.h"

class SolarSystem;



// 1 second corresponds to 1 Earth day in the simulation
class SolarSystemSimulation : public Application
{
public:
	SolarSystemSimulation(const std::string& inExecutablePath);

private:
	// As Solar System destructor is called by unique_ptr at some point in Solar System Simulation source file, 
	// and Solar System type is incomplete at this point (forward-declared), we have to implement a custom destructor
	struct SolarSystemDeleter
	{
		void operator()(SolarSystem* ptr);
	};

	std::unique_ptr<SolarSystem, SolarSystemDeleter> solarSystemScene;

	void SetUp() override;
	void Refresh() override;
};



#endif // SOLAR_SYSTEM_SIMULATION_H