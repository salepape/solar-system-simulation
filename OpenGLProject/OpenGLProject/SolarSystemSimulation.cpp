#include "SolarSystemSimulation.h"

#include "OpenGLProject/Rendering/ShaderLoader.h"
#include "SolarSystem.h"

void SolarSystemSimulation::SolarSystemDeleter::operator()(SolarSystem* ptr)
{
	delete ptr;
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