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
	Renderer renderer;
	renderer.EnableDepthTesting();
	renderer.EnableBlending();
	// @todo - Optimise rendering by activating face culling only when the controller is outside spheres
	//renderer.EnableFaceCulling();

	// Build and compile shader programs
	Shader celestialBodyShader("DefaultShader.vs", "DefaultShader.fs");
	Shader sunShader("DefaultShader.vs", "SunShader.fs");
	Shader textShader("TextShader.vs", "TextShader.fs");
	Shader beltBodyShader("InstancedModelShader.vs", "DefaultShader.fs");
	Shader skyboxShader("SkyboxShader.vs", "SkyboxShader.fs");
	Shader saturnRingsShader("DefaultShader.vs", "DefaultShader.fs");
	Shader orbitShader("DefaultShader.vs", "DefaultShader.fs");

	TextRenderer textRenderer;





	// Create Milky Way skybox
	Skybox skybox("../Textures/MilkyWay/stars.dds");

	// Load models (meshes with textures applied)
	Model saturnRings("../Models/SaturnRings.obj");
	Model asteroid("../Models/Asteroid.obj");
	Model ice("../Models/Ice.obj");

	// Initialise all celestial body meshes and their respective orbit
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

			// Apply celectial body textures to have some colour consistency
			dataInput.second.orbit = new Orbit(dataInput.second.texturePath, dataInput.second.distanceToSun);
		}

		textRenderer.LoadASCIICharacters(dataInput.first);
	}
	textRenderer.FreeFTResources();
	LoadPreComputations();

	// Use instancing to build efficiently the two rocky belts of the Solar System
	Belt asteroidBelt(	{ asteroid,	 5000,   0.05f,  10 },	{ data["Mars"].distanceToSun * 1.1f,	 2.75f * DIST_SCALE_FACTOR * 1.0f / 2.5f,  0.4f });
	Belt kuiperBelt(	{ ice,	     20000,  0.05f,  20 },	{ data["Neptune"].distanceToSun * 1.4f,  30.05f * DIST_SCALE_FACTOR,			   0.4f });





	// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun - farthest celestial body'
	std::vector<uint32_t> allShadersIDs({ celestialBodyShader.GetRendererID(), sunShader.GetRendererID(), textShader.GetRendererID(), beltBodyShader.GetRendererID(), skyboxShader.GetRendererID() });
	std::shared_ptr<Controller> controller(new Controller({ 0.0f, 100.0f, -25.0f }, { 0.0f, -25.0f, 90.0f }, 45.0f, 2.0f * data["Pluto"].distanceToSun, allShadersIDs));
	if (controller == nullptr)
	{
		return;
	}
	window->controller = controller;

	// @todo - Build a proper material for the orbits, e.g. a foggy thin tube
	std::vector<uint32_t> celestialBodyShaderID({ celestialBodyShader.GetRendererID(), orbitShader.GetRendererID() });
	Material celestialBodyMaterial;
	celestialBodyMaterial.Store(celestialBodyShaderID);
	PointLight celestialBodyLight;
	celestialBodyLight.Store(celestialBodyShaderID);

	std::vector<uint32_t> beltBodyShaderID({ beltBodyShader.GetRendererID() });
	Material beltBodyMaterial;
	beltBodyMaterial.Store(beltBodyShaderID);
	PointLight beltBodyLight;
	beltBodyLight.Store(beltBodyShaderID);

	std::vector<uint32_t> sunShaderID({ sunShader.GetRendererID() });
	Material sunMaterial({ 1.0f, 0.0f, 0.0f }, 95.0f);
	sunMaterial.Store(sunShaderID);
	DirectionalLight sunLight;
	sunLight.Store(sunShaderID);

	std::vector<uint32_t> saturnRingsShaderID({ saturnRingsShader.GetRendererID() });
	Material saturnRingsMaterial({ 0.0f, 0.0f, 0.0f }, 64.0f, 0.5f);
	saturnRingsMaterial.Store(saturnRingsShaderID);
	PointLight saturnRingsLight;
	saturnRingsLight.Store(saturnRingsShaderID);





	// RENDER LOOP (running every frame)
	while (IsNotClosed())
	{
		renderer.Clear();

		Tick();
		controller->ProcessInput(deltaTime);
		
		Camera& camera = controller->GetCamera();
		const glm::vec3& cameraPosition = camera.GetPosition();
		camera.SetPositionUniform(celestialBodyShader);
		camera.SetPositionUniform(sunShader);
		camera.SetPositionUniform(beltBodyShader);
		camera.SetProjectionViewUniform(window->GetAspectRatio());		

		// Texture sampler ID (one for each object) 
		uint32_t samplerID = 0;

		// Compute position of each celestial body so we can sort them from the farthest to the closest according to the camera, 
		// because it is needed to make blending work for multiple objects with transparency like body names and Saturn Rings
		std::unordered_map<std::string, glm::vec3> bodyPositions;
		for (auto& dataInput : data)
		{
			// Angle travelled by the planet (resp. moon) around the sun (resp. planet) since the simulation started [in radians]
			const float travelledOrbitAngle = preComputations[dataInput.first].orbitAngularFreq * elapsedTime * speedFactor;
			if (dataInput.second.parentInfo == nullptr)
			{
				dataInput.second.travelledAngle = travelledOrbitAngle;
			}

			// Store body position in Cartesian coordinates computed from Spherical ones
			glm::vec3 bodyPosition(0.0f);

			// Circular translation of satellite around corresponding planet, taking into account satellite "orbital tilt"
			if (const auto* satelliteParent = dataInput.second.parentInfo)
			{
				const float sinTravelledAngleParent = sin(satelliteParent->travelledAngle);

				bodyPosition += glm::vec3(
					preComputations[dataInput.second.parentName].distCosOrbInclination * sinTravelledAngleParent,
					preComputations[dataInput.second.parentName].distSinOrbInclination * sinTravelledAngleParent,
					satelliteParent->distanceToSun * cos(satelliteParent->travelledAngle));
			}

			// Circular translation of body around Sun, taking into account body "orbital tilt"
			const float sinTravelledAngle = sin(travelledOrbitAngle);

			bodyPosition += glm::vec3(
				preComputations[dataInput.first].distCosOrbInclination * sinTravelledAngle,
				preComputations[dataInput.first].distSinOrbInclination * sinTravelledAngle,
				dataInput.second.distanceToSun * cos(travelledOrbitAngle));

			bodyPositions.insert({ dataInput.first, bodyPosition });
		}

		std::map<float, std::string> bodiesSortedPerDist;
		for (const auto& dataInput : data)
		{
			bodiesSortedPerDist.insert({ glm::distance(cameraPosition, bodyPositions[dataInput.first]), dataInput.first });
		}





		// Draw celestial bodies (their names if required), their orbits, and animate them accordingly over time
		for (auto it = bodiesSortedPerDist.rbegin(); it != bodiesSortedPerDist.rend(); ++it)
		{
			glm::mat4 defaultModelMatrix(1.0f);

			const std::string& currentBodyName = it->second;

			// Move the body (non-constant over time) according to the circular translations computed previously
			defaultModelMatrix = glm::translate(defaultModelMatrix, bodyPositions[currentBodyName]);

			// Rotate the body (constant over time) around an axis colinear to orbit direction to reproduce its axial tilt
			defaultModelMatrix = glm::rotate(defaultModelMatrix, preComputations[currentBodyName].obliquityInRad, Utils::forwardVector);

			// Angle travelled by the celestial body around itself since the simulation started [in radians]
			const float travelledSpinAngle = preComputations[currentBodyName].spinAngularFreq * elapsedTime * speedFactor;

			// Rotate the body (non-constant over time) around axis normal to orbital plane to reproduce its spin
			defaultModelMatrix = glm::rotate(defaultModelMatrix, travelledSpinAngle, Utils::upVector);

			// Draw semi-transparent Saturn rings
			if (currentBodyName == "Saturn")
			{
				saturnRingsMaterial.SetDiffuseSamplerUniform(saturnRingsShader, samplerID);
				saturnRingsShader.Enable();
				saturnRingsShader.setUniformMat4("vu_Model", defaultModelMatrix);				
				saturnRings.Render(renderer, samplerID++);
				saturnRingsShader.Disable();
			}

			// Rotate the body (constant over time) around axis colinear to orbital plane so its poles appear vertically
			defaultModelMatrix = glm::rotate(defaultModelMatrix, Utils::halfPi, Utils::rightVector);

			if (currentBodyName == "Sun")
			{
				sunMaterial.SetDiffuseSamplerUniform(sunShader, samplerID);
				sunShader.Enable();
				sunShader.setUniformMat4("vu_Model", defaultModelMatrix);
				data[currentBodyName].sphere->Render(renderer, samplerID++);
				sunShader.Disable();
			}
			else
			{
				celestialBodyMaterial.SetDiffuseSamplerUniform(celestialBodyShader, samplerID);
				celestialBodyShader.Enable();
				celestialBodyShader.setUniformMat4("vu_Model", defaultModelMatrix);
				data[currentBodyName].sphere->Render(renderer, samplerID++);
				celestialBodyShader.Disable();
			}

			// Draw celestial body orbits
			if (currentBodyName != "Sun")
			{
				glm::mat4 orbitModelMatrix(1.0f);

				// Center the orbit (non-constant over time) around the parent planet for satellites
				if (data[currentBodyName].parentName.empty() == false)
				{
					orbitModelMatrix = glm::translate(orbitModelMatrix, bodyPositions[data[currentBodyName].parentName]);
				}

				// Rotate the orbit (constant over time) around axis colinear to orbit direction to reproduce the orbital plane
				orbitModelMatrix = glm::rotate(orbitModelMatrix, preComputations[currentBodyName].orbInclinationInRad, Utils::forwardVector);

				celestialBodyMaterial.SetDiffuseSamplerUniform(orbitShader, samplerID);
				orbitShader.Enable();
				orbitShader.setUniformMat4("vu_Model", orbitModelMatrix);
				data[currentBodyName].orbit->Render(renderer, samplerID++);
				orbitShader.Disable();
			}





			if (IsLegend())
			{
				// Orient text billboards so their readable side always faces the camera
				glm::mat4 textModelMatrix(1.0f);
				const glm::vec3& bodyPosition = bodyPositions[currentBodyName];
				const glm::vec3& forward = glm::normalize(cameraPosition - bodyPosition);
				const glm::vec3& right = glm::cross(camera.GetUp(), forward);
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
				// If the current celestial body is a satellite (i.e. has a parent)
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





		// Draw the 2 main belts of the Solar System
		camera.SetPositionUniform(beltBodyShader);

		beltBodyMaterial.SetDiffuseSamplerUniform(beltBodyShader, samplerID);
		beltBodyShader.Enable();
		asteroidBelt.Render(renderer, samplerID++);
		beltBodyShader.Disable();

		beltBodyMaterial.SetDiffuseSamplerUniform(beltBodyShader, samplerID);
		beltBodyShader.Enable();		
		kuiperBelt.Render(renderer, samplerID++);
		beltBodyShader.Disable();





		// Draw Milky Way skybox
		camera.SetInfiniteProjectionViewUniform(window->GetAspectRatio());
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

void Application::Tick()
{
	if (isPaused == false)
	{
		elapsedTime = static_cast<float>(GetTime());

		deltaTime = elapsedTime - lastFrameElapsedTime;
		lastFrameElapsedTime = elapsedTime;
	}
}