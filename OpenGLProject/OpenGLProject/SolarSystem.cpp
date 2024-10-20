#include "SolarSystem.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <map>

#include "Application.h"
#include "Billboard.h"
#include "BodyRings.h"
#include "Camera.h"
#include "Controller.h"
#include "Orbit.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "TextRenderer.h"
#include "Window.h"



SolarSystem::SolarSystem() :
	milkyWay({ "../Textures/MilkyWay/stars.dds" })
{
	ResourceLoader::LoadCelestialBodies();
	ResourceLoader::LoadBelts();

	// Fill remaining Orbit and Billboard data just after construction ended
	for (auto& celestialBody : celestialBodies)
	{
		celestialBody.orbit.SetDataPostConstruction();
		celestialBody.billboard.SetDataPostConstruction(textRenderer);
	}
	// Free FT resources once we don't have any more letters to load
	textRenderer.FreeFTResources();

	// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun - farthest celestial body'
	controller = std::make_shared<Controller>(glm::vec3(0.0f, ResourceLoader::GetBody("Sun").bodyData.radius * 1.75f, -25.0f), glm::vec3(0.0f, -25.0f, 90.0f), 45.0f, 2.0f * ResourceLoader::GetBody("Sedna").bodyData.distanceToParent);
	if (controller == nullptr)
	{
		return;
	}
	openedWindow.controller = controller;
}

void SolarSystem::Update()
{
	renderer.Clear();

	controller->ProcessKeyboardInput(runningApp.deltaTime);

	Camera& camera = controller->GetCamera();
	camera.SetProjectionViewVUniform(openedWindow.GetAspectRatio());
	camera.SetPositionFUniform();
	const glm::vec3& cameraPosition = camera.GetPosition();

	// Compute celestial body positions and sort them from farthest to closest according to the camera, 
	// because it is needed to make blending work for multiple objects with transparency, like body names and Saturn Rings
	std::map<float, uint32_t> bodiesSortedByDistance;
	for (auto& celestialBody : celestialBodies)
	{
		celestialBody.ComputeCartesianPosition(runningApp.elapsedTime * runningApp.speedFactor);
		bodiesSortedByDistance.insert({ glm::distance(cameraPosition, celestialBody.GetPosition()), celestialBody.GetID() });
	}

	for (auto bodyit = bodiesSortedByDistance.rbegin(); bodyit != bodiesSortedByDistance.rend(); ++bodyit)
	{
		// Draw celestial bodies, and animate them accordingly over time
		auto& currentBody = ResourceLoader::GetBody(bodyit->second);
		currentBody.Render(renderer, runningApp.elapsedTime * runningApp.speedFactor);

		if (runningApp.IsLegendDisplayed())
		{
			// Orient text billboards so their readable side always faces the camera
			const glm::vec3& forward = glm::normalize(cameraPosition - currentBody.GetPosition());
			const glm::vec3& right = glm::cross(camera.GetUp(), forward);

			currentBody.billboard.Render(textRenderer, currentBody.GetPosition(), forward, right);
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