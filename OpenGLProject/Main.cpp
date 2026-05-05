#include "Application/Application.h"
#include "Simulation/SolarSystem.h"

#include <filesystem>
#include <memory>
#include <iostream>



int main(int /*argc*/, char** argv)
{
	const std::filesystem::path executablePath(argv[0]);

	std::cout << "Executable path: " << executablePath.string() << std::endl;

	// 1 second corresponds to 1 Earth day in the simulation
	Application solarSystemSimulation(executablePath, "Solar System Simulation");
	solarSystemSimulation.AddScene(std::make_unique<SolarSystem>());

	solarSystemSimulation.Run();
}
