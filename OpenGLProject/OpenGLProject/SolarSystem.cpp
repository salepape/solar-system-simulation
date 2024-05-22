#include "SolarSystem.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <map>

#include "Application.h"
#include "Belt.h"
#include "Billboard.h"
#include "Camera.h"
#include "Controller.h"
#include "Orbit.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "TextRenderer.h"
#include "Window.h"



SolarSystem::SolarSystem() :
	milkyWay({ "../Textures/MilkyWay/stars.dds" }),
	saturnRings("../Models/SaturnRings.obj")
{
	ResourceLoader::LoadCelestialBodies();
	ResourceLoader::LoadBelts();

	// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun - farthest celestial body'
	controller = std::make_shared<Controller>(glm::vec3(0.0f, ResourceLoader::GetBody("Sun").radius * 1.75f, -25.0f), glm::vec3(0.0f, -25.0f, 90.0f), 45.0f, 2.0f * ResourceLoader::GetBody("Pluto").distanceToParent);
	if (controller == nullptr)
	{
		return;
	}
	openedWindow.controller = controller;

	// Load all needed quads to render celestial body names
	for (const auto& celestialBody : celestialBodies)
	{
		textRenderer.LoadASCIICharacters(celestialBody.name);
	}
	textRenderer.FreeFTResources();
}

void SolarSystem::Update()
{
	renderer.Clear();

	controller->ProcessInput(runningApp.deltaTime);

	Camera& camera = controller->GetCamera();
	camera.SetProjectionViewVUniform(openedWindow.GetAspectRatio());
	camera.SetPositionFUniforms();
	const glm::vec3& cameraPosition = camera.GetPosition();

	// Compute celestial body positions and sort them from farthest to closest according to the camera, 
	// because it is needed to make blending work for multiple objects with transparency, like body names and Saturn Rings
	std::map<float, uint32_t> bodiesSortedByDistance;
	for (auto& celestialBody : celestialBodies)
	{
		celestialBody.ComputeCartesianPosition(runningApp.elapsedTime * runningApp.speedFactor);
		bodiesSortedByDistance.insert({ glm::distance(cameraPosition, celestialBody.GetPosition()), celestialBody.ID });
	}

	for (auto bodyit = bodiesSortedByDistance.rbegin(); bodyit != bodiesSortedByDistance.rend(); ++bodyit)
	{
		// Draw celestial bodies, and animate them accordingly over time
		auto& currentBody = ResourceLoader::GetBody(bodyit->second);
		currentBody.Render(renderer, runningApp.elapsedTime * runningApp.speedFactor);

		// Draw semi-transparent Saturn rings
		if (currentBody.name == "Saturn")
		{
			saturnRings.Render(renderer);
		}

		// Draw celestial body orbits
		if (currentBody.name != "Sun")
		{
			currentBody.orbit.Render(renderer);
		}

		if (runningApp.IsLegend())
		{
			// Orient text billboards so their readable side always faces the camera
			const glm::vec3& forward = glm::normalize(cameraPosition - currentBody.GetPosition());
			const glm::vec3& right = glm::cross(camera.GetUp(), forward);

			currentBody.billboard = std::make_unique<Billboard>(currentBody.name);
			currentBody.billboard->Render(textRenderer, forward, right);
		}
	}

	// Draw the 2 main belts of the Solar System
	for (auto& belt : belts)
	{
		belt.Render(renderer);
	}

	// Draw Milky Way skybox
	camera.SetInfiniteProjectionViewVUniform(openedWindow.GetAspectRatio());
	milkyWay.Render(renderer);
}

std::vector<CelestialBody> SolarSystem::celestialBodies;
std::vector<CelestialBody>& SolarSystem::GetCelestialBodiesVector()
{
	return celestialBodies;
};

std::vector<Belt> SolarSystem::belts;
std::vector<Belt>& SolarSystem::GetBeltsVector()
{
	return belts;
};