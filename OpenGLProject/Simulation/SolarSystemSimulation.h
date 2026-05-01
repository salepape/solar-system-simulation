#ifndef SOLAR_SYSTEM_SIMULATION_H
#define SOLAR_SYSTEM_SIMULATION_H

#include <filesystem>

#include "Application/Application.h"



// 1 second corresponds to 1 Earth day in the simulation
class SolarSystemSimulation : public Application
{
public:
	SolarSystemSimulation(const std::filesystem::path& inExecutablePath);

private:
	void SetUp() override;
};



#endif // SOLAR_SYSTEM_SIMULATION_H
