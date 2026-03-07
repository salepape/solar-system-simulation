#include "SolarSystemSimulation.h"

#include "ResourceLoader.h"



void SolarSystemSimulation::SetUp()
{
	ResourceLoader::LoadShaders();

	solarSystemScene = std::make_unique<SolarSystem>();
}

void SolarSystemSimulation::Refresh()
{
	solarSystemScene->Update();
}