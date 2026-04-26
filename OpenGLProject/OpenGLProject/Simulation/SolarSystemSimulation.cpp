#include "SolarSystemSimulation.h"

#include "Rendering/ShaderLoader.h"
#include "SolarSystem.h"

void SolarSystemSimulation::SolarSystemDeleter::operator()(SolarSystem* ptr)
{
	delete ptr;
}



SolarSystemSimulation::SolarSystemSimulation(const std::filesystem::path& inExecutablePath) : Application(inExecutablePath)
{

}

void SolarSystemSimulation::SetUp()
{
	Application::SetUp();

	solarSystemScene = std::unique_ptr<SolarSystem, SolarSystemDeleter>(new SolarSystem());
}

void SolarSystemSimulation::Refresh()
{
	solarSystemScene->Update();
}
