#include "Application/Application.h"
#include "CoreEngine.h"
#include "Simulation/SolarSystem.h"

#include <filesystem>
#include <memory>
#include <iostream>



int main(int /*argc*/, char** argv)
{
	const std::filesystem::path executablePath(argv[0]);

	std::cout << "Executable path: " << executablePath.string() << std::endl;

	// 1 second corresponds to 1 Earth day in the Solar System simulation
	Application::GetInstance().SetParameters(executablePath, "Solar System Simulation");
	Application::GetInstance().SetScene(std::make_unique<SolarSystem>());

	Application::GetInstance().Run();
}
