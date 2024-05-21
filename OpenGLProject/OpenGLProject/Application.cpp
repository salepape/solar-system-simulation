#include "Application.h"

#include <iostream>
#include <glad.h>
#include <glfw3.h>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <map>

#include "Application.h"
#include "Belt.h"
#include "Billboard.h"
#include "Camera.h"
#include "Controller.h"
#include "MilkyWay.h"
#include "Orbit.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "SaturnRings.h"
#include "TextRenderer.h"
#include "Utils.h"

Application* Application::instance = nullptr;



Application::Application()
{
	window = std::make_unique<Window>(1000, 1000, "Solar System Simulation");

	// Load all OpenGL function pointers locations using GLAD
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
	{
		std::cout << "ERROR::GLAD - Failed to initialise GLAD" << std::endl;
	}

	instance = this;
}

bool Application::IsNotClosed() const
{
	return glfwWindowShouldClose(window->GLFWWindow) == 0;
}

void Application::Close() const
{
	glfwSetWindowShouldClose(window->GLFWWindow, true);
}

void Application::Pause(const bool inIsPaused)
{
	isPaused = inIsPaused;

	if (isPaused == false)
	{
		glfwSetTime(lastFrameElapsedTime);
	}
}

double Application::GetTime()
{
	return glfwGetTime();
}

void Application::Tick()
{
	if (isPaused == false)
	{
		elapsedTime = static_cast<float>(GetTime());

		deltaTime = elapsedTime - lastFrameElapsedTime;
		lastFrameElapsedTime = elapsedTime;
	}
}

void Application::Run()
{
	SimulateSolarSystem();
}

void Application::SimulateSolarSystem()
{
	// Step 1 - Build/compile shaders and their corresponding programs
	ResourceLoader::LoadShaders();

	// Step 2 - Create all scene entities (using meshes, models, etc...)
	MilkyWay milkyWay("../Textures/MilkyWay/stars.dds");

	SaturnRings saturnRings("../Models/SaturnRings.obj");

	ResourceLoader::LoadCelestialBodies();
	extern std::vector<CelestialBody> celestialBodies;

	Belt asteroidBelt({ "../Models/Asteroid.obj",  5000,     0.05f,  10 }, { ResourceLoader::GetBody("Mars").distanceToParent * 1.1f,	2.75f * ResourceLoader::DIST_SCALE_FACTOR * 1.0f / 2.5f,	0.4f });
	Belt kuiperBelt({	"../Models/Ice.obj",	   20000,	 0.05f,	 20 }, { ResourceLoader::GetBody("Neptune").distanceToParent * 1.4f,	30.05f * ResourceLoader::DIST_SCALE_FACTOR,				0.4f });

	// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun - farthest celestial body'
	std::shared_ptr<Controller> controller(new Controller({ 0.0f, ResourceLoader::GetBody("Sun").radius * 1.75f, -25.0f }, { 0.0f, -25.0f, 90.0f }, 45.0f, 2.0f * ResourceLoader::GetBody("Pluto").distanceToParent));
	if (controller == nullptr)
	{
		return;
	}
	window->controller = controller;

	// Step 3 - Create renderers for meshes and text quads
	Renderer renderer;
	renderer.EnableDepthTesting();
	renderer.EnableBlending();
	// @todo - Optimise rendering by activating face culling only when the controller is outside spheres
	//renderer.EnableFaceCulling();

	TextRenderer textRenderer(renderer);
	for (const auto& celestialBody : celestialBodies)
	{
		textRenderer.LoadASCIICharacters(celestialBody.name);
	}
	textRenderer.FreeFTResources();

	// Step 4 - RENDER LOOP (running every frame)
	while (IsNotClosed())
	{
		renderer.Clear();

		Tick();
		controller->ProcessInput(deltaTime);

		Camera& camera = controller->GetCamera();
		camera.SetProjectionViewVUniform(window->GetAspectRatio());
		camera.SetPositionFUniforms();
		const glm::vec3& cameraPosition = camera.GetPosition();

		// Compute celestial body positions and sort them from farthest to closest according to the camera, 
		// because it is needed to make blending work for multiple objects with transparency, like body names and Saturn Rings
		std::map<float, uint32_t> bodiesSortedByDistance;
		for (auto& celestialBody : celestialBodies)
		{
			celestialBody.ComputeCartesianPosition(elapsedTime * speedFactor);
			bodiesSortedByDistance.insert({ glm::distance(cameraPosition, celestialBody.GetPosition()), celestialBody.ID });
		}

		for (auto bodyit = bodiesSortedByDistance.rbegin(); bodyit != bodiesSortedByDistance.rend(); ++bodyit)
		{
			// Draw celestial bodies, and animate them accordingly over time
			auto& currentBody = ResourceLoader::GetBody(bodyit->second);
			currentBody.Render(renderer, elapsedTime * speedFactor);

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

			if (IsLegend())
			{
				// Orient text billboards so their readable side always faces the camera
				const glm::vec3& forward = glm::normalize(cameraPosition - currentBody.GetPosition());
				const glm::vec3& right = glm::cross(camera.GetUp(), forward);

				currentBody.billboard = std::make_unique<Billboard>(currentBody.name);
				currentBody.billboard->Render(textRenderer, forward, right);
			}
		}

		// Draw the 2 main belts of the Solar System
		asteroidBelt.Render(renderer);
		kuiperBelt.Render(renderer);

		// Draw Milky Way skybox
		camera.SetInfiniteProjectionViewVUniform(window->GetAspectRatio());
		milkyWay.Render(renderer);

		window->SwapBuffers();
		window->PollEvents();
	}

	// Step 5 - Free up all rendering resources
	//renderer.DisableFaceCulling();
	renderer.DisableBlending();
	renderer.DisableDepthTesting();

	window->FreeUpResources();
}

void Application::ChangeSpeed(const float inSpeedFactor)
{
	speedFactor *= inSpeedFactor;
}