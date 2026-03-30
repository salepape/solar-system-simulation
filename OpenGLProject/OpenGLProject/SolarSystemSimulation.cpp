#include "SolarSystemSimulation.h"

#include "ResourceLoader.h"



void SolarSystemSimulation::SetUp()
{
	ShaderLoader::BuildShaders();

	solarSystemScene = std::make_unique<SolarSystem>();
}

void SolarSystemSimulation::Refresh()
{
	solarSystemScene->Update();
}