#include "Simulation/SolarSystemSimulation.h"

#include <iostream>
#include <string>



int main(int argc, char** argv)
{
	const std::string executablePath(argv[0]);

	std::cout << "Executable path: " << executablePath << std::endl;

	SolarSystemSimulation app(executablePath);
	app.Run();
}