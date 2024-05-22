#include "SolarSystemSimulation.h"

#include "ResourceLoader.h"
#include "SolarSystem.h"



void SolarSystemSimulation::SetUp()
{
	ResourceLoader::LoadShaders();

	scene = std::make_unique<SolarSystem>();
}

void SolarSystemSimulation::Refresh()
{
	scene->Update();
}