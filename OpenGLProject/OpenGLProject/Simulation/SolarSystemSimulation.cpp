#include "SolarSystemSimulation.h"

#include "Rendering/ShaderLoader.h"
#include "SolarSystem.h"

void SolarSystemSimulation::SolarSystemDeleter::operator()(SolarSystem* ptr)
{
	delete ptr;
}



SolarSystemSimulation::SolarSystemSimulation(const std::string& inExecutablePath) : Application(inExecutablePath)
{

}

void SolarSystemSimulation::SetUp()
{
	ShaderLoader::BuildShaders();

	solarSystemScene = std::unique_ptr<SolarSystem, SolarSystemDeleter>(new SolarSystem());
}

void SolarSystemSimulation::Refresh()
{
	solarSystemScene->Update();
}