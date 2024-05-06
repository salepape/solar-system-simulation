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

#include "Application.h"
#include "Belt.h"
#include "Camera.h"
#include "Controller.h"
#include "Data.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"
#include "Model.h"
#include "Orbit.h"
#include "Renderer.h"
#include "Shader.h"
#include "Skybox.h"
#include "Sphere.h"
#include "TextRenderer.h"
#include "Utils.h"
#include "Window.h"

Application* Application::instance = nullptr;



Application::Application()
{
	window = new Window(1000, 1000, "Solar System Simulation");

	// Load all OpenGL function pointers locations using GLAD
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
	{
		std::cout << "ERROR::GLAD - Failed to initialise GLAD" << std::endl;
	}

	instance = this;
}

Application::~Application()
{
	delete window;
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
		glfwSetTime(lastFrame);
	}
}

void Application::SimulateSolarSystem()
{
	Renderer renderer;
	renderer.EnableDepthTesting();
	renderer.EnableBlending();
	// @todo - Optimise rendering by activating face culling only when the controller is outside spheres
	//renderer.EnableFaceCulling();

	// Build and compile shader programs
	Shader defaultShader("DefaultShader.vs", "DefaultShader.fs");
	Shader sunShader("DefaultShader.vs", "SunShader.fs");
	Shader textShader("TextShader.vs", "TextShader.fs");
	Shader instancedModelShader("InstancedModelShader.vs", "DefaultShader.fs");
	Shader skyboxShader("SkyboxShader.vs", "SkyboxShader.fs");

	TextRenderer textRenderer;





	// Create Milky Way skybox
	Skybox skybox("../Textures/MilkyWay/stars.dds");

	// Load models (meshes with textures applied)
	Model saturnRings("../Models/SaturnRings.obj");
	Model asteroid("../Models/Asteroid.obj");
	Model ice("../Models/Ice.obj");

	// Initialise all celestial bodies and their respective orbit (applying celectial body textures to have some colour consistency)
	LoadData();
	for (auto& dataInput : data)
	{
		if (dataInput.first == "Sun")
		{
			dataInput.second.sphere = new Sphere(dataInput.second.texturePath, dataInput.second.radius * 0.5f);
		}
		else
		{
			dataInput.second.sphere = new Sphere(dataInput.second.texturePath, dataInput.second.radius);
			dataInput.second.orbit = new Orbit(dataInput.second.texturePath, dataInput.second.dist);
		}

		textRenderer.LoadASCIICharacters(dataInput.first);
	}
	textRenderer.FreeFTResources();
	LoadPreComputations();

	// Do some instancing to build the main Solar Systems rock belts
	Belt asteroidBelt(	{ asteroid, 5000,  0.05f, 10 },		{ data["Mars"].dist * 1.1f,	  2.75f * DIST_SCALE_FACTOR * 1.0f / 2.5f,	0.4f });
	Belt kuiperBelt(	{ ice,		 20000, 0.05f, 20 },	{ data["Neptune"].dist * 1.4f, 30.05f * DIST_SCALE_FACTOR,				0.4f });





	// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun - farthest celestial body'
	std::vector<uint32_t> allShadersIDs({ defaultShader.GetRendererID(), sunShader.GetRendererID(), textShader.GetRendererID(), instancedModelShader.GetRendererID(), skyboxShader.GetRendererID() });
	Controller controller({ 0.0f, 100.0f, -25.0f }, { 0.0f, -25.0f, 90.0f }, 45.0f, 2.0f * data["Pluto"].dist, allShadersIDs);
	window->controller = &controller;

	std::vector<uint32_t> celestialBodyShadersIDs({ defaultShader.GetRendererID(), instancedModelShader.GetRendererID() });
	Material celestialBodyMaterial;
	celestialBodyMaterial.Store(celestialBodyShadersIDs);
	PointLight celestialBodyLight;
	celestialBodyLight.Store(celestialBodyShadersIDs);

	std::vector<uint32_t> sunShaderID({ sunShader.GetRendererID() });
	Material sunMaterial({ 1.0f, 0.0f, 0.0f }, 95.0f);
	sunMaterial.Store(sunShaderID);
	DirectionalLight sunLight;
	sunLight.Store(sunShaderID);





	// RENDER LOOP (running every frame)
	while (IsNotClosed())
	{
		UpdateFrames();
		controller.ProcessInput(deltaTime);

		renderer.Clear();

		std::unordered_map<std::string, glm::vec3> bodyPositions;
		const glm::vec3& cameraPosition = controller.GetCamera().GetPosition();

		controller.GetCamera().SetPositionUniform(defaultShader);
		controller.GetCamera().SetPositionUniform(sunShader);
		controller.GetCamera().SetPositionUniform(instancedModelShader);
		controller.GetCamera().SetProjectionViewUniform(window->GetAspectRatio());

		// Texture sampler ID (one for each object) 
		uint32_t samplerID = 0;





		// Draw celestial bodies, their orbits and their motion
		for (auto& dataInput : data)
		{
			const float frameRate = GetFrameRate();

			// Angle of rotation around the sun (resp. planet) for planets (resp. moons) per frame [in radians]
			const float angleRot = preComputations[dataInput.first].angleRotCst * frameRate;
			if (dataInput.second.parentInfo == nullptr)
			{
				dataInput.second.angleRot = angleRot;
			}

			// Angle of rotation of the celestial body around itself per frame [in radians]
			const float angleRotItself = preComputations[dataInput.first].rotPeriodCst * frameRate;

			// Simulate movements that affects the current celestial bodies
			glm::mat4 defaultModelMatrix(1.0f);
			glm::mat4 orbitModelMatrix(1.0f);

			// Circular translation of satellite around corresponding planet
			if (const auto& satelliteParent = dataInput.second.parentInfo)
			{
				const float sinParentAngleRot = sin(satelliteParent->angleRot);

				defaultModelMatrix = glm::translate(defaultModelMatrix, glm::vec3(
					preComputations[dataInput.second.parentName].cosCircularTslCst * sinParentAngleRot,
					preComputations[dataInput.second.parentName].sinCircularTslCst * sinParentAngleRot,
					satelliteParent->dist * cos(satelliteParent->angleRot)));
				orbitModelMatrix = defaultModelMatrix;
			}

			// Orbital tilt (around axis colinear to orbit direction) + Circular translation along the orbit (equidistance to axis normal to orbital plane)
			const float sinAngleRot = sin(angleRot);

			defaultModelMatrix = glm::translate(defaultModelMatrix, glm::vec3(
				preComputations[dataInput.first].cosCircularTslCst * sinAngleRot,
				preComputations[dataInput.first].sinCircularTslCst * sinAngleRot,
				dataInput.second.dist * cos(angleRot)));

			bodyPositions.insert({ dataInput.first, defaultModelMatrix[3] });

			// Axis tilt (around axis colinear to orbit direction)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, preComputations[dataInput.first].obliquityInRad, Utils::rightVector);

			// Rotation on itself (around axis normal to orbital plane)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, angleRotItself, Utils::upVector);

			// Draw Saturn rings
			if (dataInput.first == "Saturn")
			{
				defaultShader.Enable();
				defaultShader.setUniformMat4("vu_Model", defaultModelMatrix);
				defaultShader.setUniformInt("fu_DiffuseMat", samplerID);
				saturnRings.Render(renderer, samplerID++);
				defaultShader.Disable();
			}

			// Rotation on itself (to have celestial body poles vertical)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, Utils::halfPi, Utils::rightVector);

			if (dataInput.first == "Sun")
			{
				sunShader.Enable();
				sunShader.setUniformInt("fu_DiffuseMat", samplerID);
				sunShader.Disable();
			}
			else
			{
				defaultShader.Enable();
				defaultShader.setUniformInt("fu_DiffuseMat", samplerID);
				defaultShader.Disable();
			}

			// Spherical celestial bodies
			if (const auto& sphere = dataInput.second.sphere)
			{
				if (dataInput.first == "Sun")
				{
					sunShader.Enable();
					sunShader.setUniformMat4("vu_Model", defaultModelMatrix);
					sphere->Render(renderer, samplerID++);
					sunShader.Disable();
				}
				else
				{
					defaultShader.Enable();
					defaultShader.setUniformMat4("vu_Model", defaultModelMatrix);
					sphere->Render(renderer, samplerID++);
					defaultShader.Disable();
				}
			}

			// Draw planet orbits
			if (dataInput.first != "Sun")
			{
				orbitModelMatrix = glm::rotate(orbitModelMatrix, preComputations[dataInput.first].orbInclinationInRad, Utils::backVector);

				defaultShader.Enable();
				defaultShader.setUniformMat4("vu_Model", orbitModelMatrix);
				defaultShader.setUniformInt("fu_DiffuseMat", samplerID);
				dataInput.second.orbit->Render(renderer, samplerID++);
				defaultShader.Disable();
			}
		}





		// Draw meshes with transparency (only billboard containing current celestial body name on top of the mesh/model for now)
		if (IsLegend())
		{
			// Be sure we render names from the farthest to the closest celestial body to the camera to make blending work for multiple objects
			std::map<float, std::string> bodiesSortedPerDist;
			for (const auto& dataInput : data)
			{
				bodiesSortedPerDist.insert({ glm::distance(cameraPosition, bodyPositions[dataInput.first]), dataInput.first });
			}

			for (auto it = bodiesSortedPerDist.rbegin(); it != bodiesSortedPerDist.rend(); ++it)
			{
				const std::string& currentBodyName = it->second;

				// Orient text to camera position
				glm::mat4 textModelMatrix(1.0f);
				const glm::vec3& bodyPosition = bodyPositions[currentBodyName];
				const glm::vec3& forward = glm::normalize(cameraPosition - bodyPosition);
				const glm::vec3& right = glm::cross(controller.GetCamera().GetUp(), forward);
				const glm::vec3& up = cross(forward, right);
				textModelMatrix[0] = glm::vec4(right, 0.0f);
				textModelMatrix[1] = glm::vec4(up, 0.0f);
				textModelMatrix[2] = glm::vec4(forward, 0.0f);
				textModelMatrix[3] = glm::vec4(bodyPosition, 1.0f);

				textShader.Enable();
				textShader.setUniformMat4("vu_Model", textModelMatrix);
				textShader.setUniformInt("fu_DiffuseMat", samplerID);
				textShader.setUniformVec3("fu_DiffuseColour", Utils::whiteColour);

				float textHeight = 0.0f;
				float textScale = 0.0f;
				if (currentBodyName == "Sun")
				{
					textHeight = preComputations[currentBodyName].sunTextHeight;
					textScale = preComputations[currentBodyName].sunTextScale;
				}
				// If this is a satellite (i.e. has a parent)
				else if (data[currentBodyName].parentInfo)
				{
					textHeight = preComputations[currentBodyName].satelliteTextHeight;
					textScale = preComputations[currentBodyName].satelliteTextScale;
				}
				else
				{
					textHeight = preComputations[currentBodyName].textHeight;
					textScale = preComputations[currentBodyName].textScale;
				}
				textRenderer.Render(renderer, currentBodyName, 0.0f, textHeight, textScale, samplerID++);

				textShader.Disable();
			}
		}





		// Draw the 2 main belts composed
		instancedModelShader.Enable();
		instancedModelShader.setUniformVec3("fu_CameraPosition", cameraPosition);
		instancedModelShader.setUniformInt("fu_DiffuseMat", samplerID);
		asteroidBelt.Render(renderer, samplerID++);
		instancedModelShader.setUniformInt("fu_DiffuseMat", samplerID);
		kuiperBelt.Render(renderer, samplerID++);
		instancedModelShader.Disable();





		// Draw Milky Way skybox
		controller.GetCamera().SetInfiniteProjectionViewUniform(window->GetAspectRatio());
		skyboxShader.Enable();
		skyboxShader.setUniformInt("fu_DiffuseMat", samplerID);
		renderer.SetDepthFctToEqual();
		skybox.Render(renderer, samplerID++);
		renderer.SetDepthFctToLess();
		skyboxShader.Disable();





		window->SwapBuffers();
		window->PollEvents();
	}

	//renderer.DisableFaceCulling();
	renderer.DisableBlending();
	renderer.DisableDepthTesting();

	window->FreeUpResources();
}

double Application::GetTime()
{
	return glfwGetTime();
}

void Application::UpdateFrames()
{
	if (isPaused == false)
	{
		// Time elapsed since GLFW initialisation [considered as a dimensionless chrono, but in seconds in reality]
		currentFrame = static_cast<float>(GetTime());

		// Compute delta time in order to reduce differences between computer processing powers
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
}

float Application::GetFrameRate() const
{
	return currentFrame * speedFactor;
}