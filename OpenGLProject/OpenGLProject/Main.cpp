#include "Simulation/SolarSystemSimulation.h"

#include <filesystem>
#include <iostream>



int main(int /*argc*/, char** argv)
{
	const std::filesystem::path executablePath(argv[0]);

	std::cout << "Executable path: " << executablePath.string() << std::endl;

	SolarSystemSimulation app(executablePath);
	app.Run();
}
