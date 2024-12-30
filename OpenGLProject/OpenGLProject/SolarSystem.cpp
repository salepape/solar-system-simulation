#include "SolarSystem.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <map>

#include "Application.h"
#include "Billboard.h"
#include "BodyRings.h"
#include "Orbit.h"
#include "PerspectiveCamera.h"
#include "PerspectiveCameraController.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Spacecraft.h"
#include "TextRenderer.h"
#include "Window.h"



SolarSystem::SolarSystem() :
	milkyWay({ "../Textures/MilkyWay/stars.dds" })
{
	ResourceLoader::LoadAssets();

	for (const BodySystem& bodySystem : bodySystems)
	{
		// Texture creation is handled by the Text Renderer for now (glyph rendering issue when textures created otherwise)
		textRenderer.LoadASCIICharacters(bodySystem.celestialBody.GetName());
	}
	// Free FT resources once we don't have any more letters to load
	textRenderer.FreeFTResources();

	// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun - farthest celestial body'
	spacecraft = std::make_shared<Spacecraft>(glm::vec3(0.0f, ResourceLoader::GetBodySystem("Sun").celestialBody.bodyData.radius * 1.75f, -25.0f), glm::vec3(0.0f, -25.0f, 90.0f), 45.0f, 2.0f * ResourceLoader::GetBodySystem("Sedna").celestialBody.bodyData.distanceToParent);
}

void SolarSystem::Update()
{
	renderer.Clear();

	PerspectiveCameraController& cameraController = spacecraft->cameraController;
	cameraController.ProcessKeyboardInput(runningApp.deltaTime);

	PerspectiveCamera& camera = cameraController.GetCamera();
	camera.SetProjectionViewVUniform(openWindow.GetAspectRatio());
	camera.SetPositionFUniform();
	const glm::vec3& cameraPosition = camera.GetPosition();

	// Compute celestial body positions and sort them from farthest to closest according to the camera, 
	// because it is needed to make blending work for multiple objects with transparency, like body names and Saturn Rings
	std::map<float, uint32_t> bodiesSortedByDistance;
	for (BodySystem& bodySystem : bodySystems)
	{
		bodySystem.celestialBody.ComputeCartesianPosition(runningApp.elapsedTime * runningApp.speedFactor);
		bodiesSortedByDistance.insert({ glm::distance(cameraPosition, bodySystem.celestialBody.GetPosition()), bodySystem.celestialBody.GetID() });
	}

	for (auto bodyit = bodiesSortedByDistance.rbegin(); bodyit != bodiesSortedByDistance.rend(); ++bodyit)
	{
		// Draw celestial bodies, and animate them accordingly over time
		BodySystem& bodySystem = ResourceLoader::GetBodySystem(bodyit->second);
		bodySystem.Render(renderer, runningApp.elapsedTime * runningApp.speedFactor);

		if (runningApp.IsLegendDisplayed())
		{
			// Orient text billboards so their readable side always faces the camera
			const glm::vec3& forward = glm::normalize(cameraPosition - bodySystem.celestialBody.GetPosition());
			const glm::vec3& right = glm::cross(camera.GetUp(), forward);

			bodySystem.billboard.Render(textRenderer, bodySystem.celestialBody.GetPosition(), forward, right);
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

std::vector<BodySystem> SolarSystem::bodySystems;
std::vector<BodySystem>& SolarSystem::GetBodySystemsVector()
{
	return bodySystems;
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