#include "SolarSystem.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <map>

#include "Application.h"
#include "Billboard.h"
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
	ResourceLoader::LoadAssets();

	// Fill remaining Orbit and Billboard data just after construction ended
	for (CelestialBody& body : celestialBodies)
	{
		body.orbit.SetDataPostConstruction();
		body.billboard.SetDataPostConstruction(textRenderer);
	}
	// Free FT resources once we don't have any more letters to load
	textRenderer.FreeFTResources();

	// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun - farthest celestial body'
	controller = std::make_shared<Controller>(glm::vec3(0.0f, ResourceLoader::GetBody("Sun").bodyData.radius * 1.75f, -25.0f), glm::vec3(0.0f, -25.0f, 90.0f), 45.0f, 2.0f * ResourceLoader::GetBody("Sedna").bodyData.distanceToParent);
	if (controller == nullptr)
	{
		return;
	}
	openWindow.controller = controller;
}

void SolarSystem::Update()
{
	renderer.Clear();

	controller->ProcessKeyboardInput(runningApp.deltaTime);

	Camera& camera = controller->GetCamera();
	camera.SetProjectionViewVUniform(openWindow.GetAspectRatio());
	camera.SetPositionFUniform();
	const glm::vec3& cameraPosition = camera.GetPosition();

	// Compute celestial body positions and sort them from farthest to closest according to the camera, 
	// because it is needed to make blending work for multiple objects with transparency, like body names and Saturn Rings
	std::map<float, uint32_t> bodiesSortedByDistance;
	for (CelestialBody& body : celestialBodies)
	{
		body.ComputeCartesianPosition(runningApp.elapsedTime * runningApp.speedFactor);
		bodiesSortedByDistance.insert({ glm::distance(cameraPosition, body.GetPosition()), body.GetID() });
	}

	for (auto bodyit = bodiesSortedByDistance.rbegin(); bodyit != bodiesSortedByDistance.rend(); ++bodyit)
	{
		// Draw celestial bodies, and animate them accordingly over time
		CelestialBody& body = ResourceLoader::GetBody(bodyit->second);
		body.Render(renderer, runningApp.elapsedTime * runningApp.speedFactor);

		if (runningApp.IsLegendDisplayed())
		{
			// Orient text billboards so their readable side always faces the camera
			const glm::vec3& forward = glm::normalize(cameraPosition - body.GetPosition());
			const glm::vec3& right = glm::cross(camera.GetUp(), forward);

			body.billboard.Render(textRenderer, body.GetPosition(), forward, right);
		}
	}

	// Draw the 2 main belts of the Solar System
	for (Belt& belt : belts)
	{
		belt.Render(renderer);
	}

	// Draw Milky Way skybox
	camera.SetInfiniteProjectionViewVUniform(openWindow.GetAspectRatio());
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

std::vector<BodyRings> SolarSystem::rings;
std::vector<BodyRings>& SolarSystem::GetRingsVector()
{
	return rings;
};