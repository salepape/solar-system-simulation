#ifndef SOLAR_SYSTEM_SIMULATION_H
#define SOLAR_SYSTEM_SIMULATION_H

#include <memory>

#include "Application.h"
#include "SolarSystem.h"



// 1 second corresponds to 1 Earth day in the simulation
class SolarSystemSimulation : public Application
{
public:
	void SetUp() override;
	void Refresh() override;

private:
	std::unique_ptr<SolarSystem> scene;
};



#endif // SOLAR_SYSTEM_SIMULATION_H