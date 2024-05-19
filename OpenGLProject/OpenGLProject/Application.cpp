#include "Application.h"

#include <iostream>
#include <glad.h>
#include <glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <map>
#include <unordered_map>

#include "Application.h"
#include "Belt.h"
#include "Camera.h"
#include "Controller.h"
#include "MilkyWay.h"
#include "Orbit.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "SaturnRings.h"
#include "Shader.h"
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

void Application::Run()
{
	SimulateSolarSystem();
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

void Application::ChangeSpeed(const float inSpeedFactor)
{
	speedFactor *= inSpeedFactor;
}

void Application::SimulateSolarSystem()
{
	// Step 1 - Build/compile shaders and their corresponding programs
	ResourceLoader::LoadShaders();
	Shader& celestialBodyShader = ResourceLoader::GetShader("CelestialBody");
	Shader& beltBodyShader = ResourceLoader::GetShader("BeltBody");
	Shader& sunShader = ResourceLoader::GetShader("Sun");
	Shader& textShader = ResourceLoader::GetShader("Text");
	Shader& saturnRingsShader = ResourceLoader::GetShader("SaturnRings");
	Shader& orbitShader = ResourceLoader::GetShader("Orbit");





	// Step 2 - Create all scene entities (using meshes, models, etc...)
	MilkyWay milkyWay("../Textures/MilkyWay/stars.dds");

	SaturnRings saturnRings("../Models/SaturnRings.obj");

	ResourceLoader::LoadCelestialBodies();
	extern std::vector<CelestialBody> celestialBodies;

	Belt asteroidBelt({ "../Models/Asteroid.obj",  5000,     0.05f,  10 }, { ResourceLoader::GetBody("Mars").distanceToParent * 1.1f,	2.75f * ResourceLoader::DIST_SCALE_FACTOR * 1.0f / 2.5f,	0.4f });
	Belt kuiperBelt({	"../Models/Ice.obj",	   20000,	 0.05f,	 20 }, { ResourceLoader::GetBody("Neptune").distanceToParent * 1.4f,	30.05f * ResourceLoader::DIST_SCALE_FACTOR,				0.4f });

	// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun - farthest celestial body'
	std::vector<uint32_t> allShadersIDs({ celestialBodyShader.GetRendererID(), beltBodyShader.GetRendererID(), sunShader.GetRendererID(), ResourceLoader::GetShader("Skybox").GetRendererID(), textShader.GetRendererID(), saturnRingsShader.GetRendererID(), orbitShader.GetRendererID() });
	std::shared_ptr<Controller> controller(new Controller({ 0.0f, ResourceLoader::GetBody("Sun").radius * 1.75f, -25.0f }, { 0.0f, -25.0f, 90.0f }, 45.0f, 2.0f * ResourceLoader::GetBody("Pluto").distanceToParent, allShadersIDs));
	if (controller == nullptr)
	{
		return;
	}
	window->controller = controller;





	// Step 3 - Create renderers for scene entities and text
	Renderer renderer;
	renderer.EnableDepthTesting();
	renderer.EnableBlending();
	// @todo - Optimise rendering by activating face culling only when the controller is outside spheres
	//renderer.EnableFaceCulling();

	TextRenderer textRenderer;
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
		camera.SetPositionFUniform(celestialBodyShader);
		camera.SetPositionFUniform(sunShader);
		camera.SetPositionFUniform(saturnRingsShader);
		camera.SetPositionFUniform(orbitShader);
		const glm::vec3& cameraPosition = camera.GetPosition();

		// Texture sampler ID (one for each object)
		uint32_t samplerID = 0;

		// Compute position of each celestial body so we can sort them from the farthest to the closest according to the camera, 
		// because it is needed to make blending work for multiple objects with transparency like body names and Saturn Rings
		std::unordered_map<uint32_t, glm::vec3> bodyPositions;
		for (auto& celestialBody : celestialBodies)
		{
			// Angle travelled by the planet (resp. moon) around the sun (resp. planet) since the simulation started [in radians]
			const float travelledOrbitAngle = celestialBody.preComputations.orbitAngularFreq * elapsedTime * speedFactor;
			if (celestialBody.parentID == -1)
			{
				celestialBody.travelledAngle = travelledOrbitAngle;
			}

			// Store body position in Cartesian coordinates computed from Spherical ones
			glm::vec3 bodyPosition(0.0f);

			// Circular translation of satellite around corresponding planet, taking into account satellite "orbital tilt"
			if (celestialBody.parentID != -1)
			{
				const auto& satelliteParent = ResourceLoader::GetBody(celestialBody.parentID);

				const float sinTravelledAngleParent = glm::sin(satelliteParent.travelledAngle);

				bodyPosition += glm::vec3(
					satelliteParent.preComputations.distCosOrbInclination * sinTravelledAngleParent,
					satelliteParent.preComputations.distSinOrbInclination * sinTravelledAngleParent,
					satelliteParent.distanceToParent * glm::cos(satelliteParent.travelledAngle));
			}

			// Circular translation of body around Sun, taking into account body "orbital tilt"
			const float sinTravelledAngle = glm::sin(travelledOrbitAngle);

			bodyPosition += glm::vec3(
				celestialBody.preComputations.distCosOrbInclination * sinTravelledAngle,
				celestialBody.preComputations.distSinOrbInclination * sinTravelledAngle,
				celestialBody.distanceToParent * glm::cos(travelledOrbitAngle));

			bodyPositions.insert({ celestialBody.ID, bodyPosition });
		}

		std::map<float, uint32_t> bodiesSortedPerDist;
		for (const auto& celestialBody : celestialBodies)
		{
			bodiesSortedPerDist.insert({ glm::distance(cameraPosition, bodyPositions[celestialBody.ID]), celestialBody.ID });
		}





		// Draw celestial bodies (their names if required), their orbits, and animate them accordingly over time
		for (auto it = bodiesSortedPerDist.rbegin(); it != bodiesSortedPerDist.rend(); ++it)
		{
			glm::mat4 defaultModelMatrix(1.0f);

			const uint32_t currentBodyID = it->second;
			const auto& currentBodyPosition = bodyPositions[currentBodyID];
			auto& currentBody = ResourceLoader::GetBody(currentBodyID);

			// Move the body (non-constant over time) according to the circular translations computed previously
			defaultModelMatrix = glm::translate(defaultModelMatrix, currentBodyPosition);

			// Rotate the body (constant over time) around an axis colinear to orbit direction to reproduce its axial tilt
			defaultModelMatrix = glm::rotate(defaultModelMatrix, currentBody.preComputations.obliquityInRad, Utils::forwardVector);

			// Angle travelled by the celestial body around itself since the simulation started [in radians]
			const float travelledSpinAngle = currentBody.preComputations.spinAngularFreq * elapsedTime * speedFactor;

			// Rotate the body (non-constant over time) around axis normal to orbital plane to reproduce its spin
			defaultModelMatrix = glm::rotate(defaultModelMatrix, travelledSpinAngle, Utils::upVector);

			// Draw semi-transparent Saturn rings
			if (currentBody.name == "Saturn")
			{	
				saturnRings.Render(renderer, samplerID, defaultModelMatrix);
			}

			// Rotate the body (constant over time) around axis colinear to orbital plane so its poles appear vertically
			defaultModelMatrix = glm::rotate(defaultModelMatrix, Utils::halfPi, Utils::rightVector);

			currentBody.Render(renderer, samplerID, defaultModelMatrix);





			// Draw celestial body orbits
			if (currentBody.name != "Sun")
			{
				glm::mat4 orbitModelMatrix(1.0f);

				// Center the orbit (non-constant over time) around the parent planet for satellites
				if (currentBody.parentID != -1)
				{
					orbitModelMatrix = glm::translate(orbitModelMatrix, bodyPositions[currentBody.parentID]);
				}

				// Rotate the orbit (constant over time) around axis colinear to orbit direction to reproduce the orbital plane
				orbitModelMatrix = glm::rotate(orbitModelMatrix, currentBody.preComputations.orbInclinationInRad, Utils::forwardVector);

				currentBody.orbit.Render(renderer, samplerID, orbitModelMatrix);
			}





			if (IsLegend())
			{
				// Orient text billboards so their readable side always faces the camera
				glm::mat4 textModelMatrix(1.0f);
				const glm::vec3& bodyPosition = currentBodyPosition;
				const glm::vec3& forward = glm::normalize(cameraPosition - bodyPosition);
				const glm::vec3& right = glm::cross(camera.GetUp(), forward);
				const glm::vec3& up = cross(forward, right);
				textModelMatrix[0] = glm::vec4(right, 0.0f);
				textModelMatrix[1] = glm::vec4(up, 0.0f);
				textModelMatrix[2] = glm::vec4(forward, 0.0f);
				textModelMatrix[3] = glm::vec4(bodyPosition, 1.0f);

				// @todo - Create a class Billboard inheriting from SceneEntity as a set of Quad components being the one defined in TextRenderer
				textShader.Enable();
				textShader.setUniformMat4("vu_Model", textModelMatrix);
				textShader.setUniformInt("fu_DiffuseMat", samplerID);
				textShader.setUniformVec3("fu_DiffuseColour", Utils::whiteColour);
				textRenderer.Render(renderer, currentBody.name, 0.0f, currentBody.preComputations.textHeight, currentBody.preComputations.textScale, samplerID++);
				textShader.Disable();
			}
		}





		// Draw the 2 main belts of the Solar System
		camera.SetPositionFUniform(beltBodyShader);
		asteroidBelt.Render(renderer, samplerID);
		kuiperBelt.Render(renderer, samplerID);





		// Draw Milky Way skybox
		camera.SetInfiniteProjectionViewVUniform(window->GetAspectRatio());
		milkyWay.Render(renderer, samplerID);





		window->SwapBuffers();
		window->PollEvents();
	}





	// Step 5 - Free up all rendering resources
	//renderer.DisableFaceCulling();
	renderer.DisableBlending();
	renderer.DisableDepthTesting();

	window->FreeUpResources();
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