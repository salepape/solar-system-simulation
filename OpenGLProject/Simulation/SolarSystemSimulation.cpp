#include "SolarSystemSimulation.h"

#include <memory>

#include "SolarSystem.h"



SolarSystemSimulation::SolarSystemSimulation(const std::filesystem::path& inExecutablePath) :
	Application(inExecutablePath)
{

}

void SolarSystemSimulation::SetUp()
{
	Application::SetUp();

	scene = std::make_unique<SolarSystem>();
}
