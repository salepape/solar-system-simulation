#include <iostream>
#include <glad.h>
#include <glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

#include "Belt.h"
#include "Camera.h"
#include "Data.h"
#include "Light.h"
#include "Material.h"
#include "Model.h"
#include "Orbit.h"
#include "Renderer.h"
#include "Shader.h"
#include "Skybox.h"
#include "Sphere.h"
#include "TextRenderer.h"
#include "UniformBuffer.h"
#include "Utils.h"
#include "Window.h"





int main()
{
	Window window(1000, 1000, "Solar System Simulation");
	const float aspectRatio = window.GetAspectRatio();

	Camera camera(glm::vec3(0.0f, 50.0f, 200.0f));
	window.camera = &camera;

	// Load all OpenGL function pointers locations using GLAD
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
	{
		std::cout << "ERROR::GLAD - Failed to initialise GLAD" << std::endl;
		return -1;
	}





	// Build and compile shader programs
	Shader defaultShader("DefaultShader.vs", "DefaultShader.fs");
	Shader sunShader("SunShader.vs", "SunShader.fs");
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
	Belt asteroidBelt	( {asteroid, 5000,  0.05f, 10},	{data["Mars"].dist * 1.1f,	  2.75f * DIST_SCALE_FACTOR * 1.0f / 2.5f,	0.4f} );
	Belt kuiperBelt		( {ice,		 20000, 0.05f, 20},	{data["Neptune"].dist * 1.4f, 30.05f * DIST_SCALE_FACTOR,				0.4f} );





	std::vector<unsigned int> matricesShadersIDs({ defaultShader.GetRendererID(), sunShader.GetRendererID(), textShader.GetRendererID(), instancedModelShader.GetRendererID(), skyboxShader.GetRendererID() });
	UniformBuffer uboMatrices(matricesShadersIDs, "matrices", static_cast<size_t>(2 * Utils::mat4v4Size));

	std::vector<unsigned int> entitiesShadersIDs({ defaultShader.GetRendererID(), instancedModelShader.GetRendererID() });

	Material material;
	material.Store(entitiesShadersIDs);

	Light light;
	light.Store(entitiesShadersIDs);





	Renderer renderer;
	renderer.DepthTest();

	// RENDER LOOP (running every frame)
	while (window.DoNotClose())
	{
		window.UpdateFrames();
		window.ProcessInput(camera);

		renderer.Clear();
		renderer.Blend();

		const glm::vec3& cameraPosition = camera.GetPosition();

		defaultShader.Enable();
		defaultShader.setUniformVec3("viewPos", cameraPosition);
		defaultShader.Disable();

		// Simulate a zoom - set far plane variable to a sufficiently high value 
		const glm::mat4& projectionMatrix = glm::perspective(glm::radians(camera.GetZoom()), aspectRatio, 0.1f, 1000.0f);

		// Simulate a camera circling around the scene 
		const glm::mat4& viewMatrix = camera.GetViewMatrix();

		uboMatrices.InitSubData({
			{ static_cast<const void*>(glm::value_ptr(projectionMatrix)), Utils::mat4v4Size },
			{ static_cast<const void*>(glm::value_ptr(viewMatrix)), Utils::mat4v4Size }
			});

		// Texture sampler ID (one for each object) 
		unsigned int samplerID = 0;

		// Draw celestial bodies, their orbits and their motion
		for (auto& dataInput : data)
		{
			// Both angles below are in radians
			float angleRot = 0.0f;
			float angleRotItself = 0.0f;

			if (dataInput.first != "Sun")
			{
				const float frameRate = window.GetFrameRate();

				// Angle of rotation around the sun (resp. planet) for planets (resp. moons) per frame
				angleRot = preComputations[dataInput.first].angleRotCst * frameRate;
				if (dataInput.second.parentInfo == nullptr)
				{
					dataInput.second.angleRot = angleRot;
				}

				// Angle of rotation of the celestial body around itself per frame
				angleRotItself = preComputations[dataInput.first].rotPeriodCst * frameRate;
			}

			// Simulate movements that affects the current celestial bodies
			glm::mat4 defaultModelMatrix(1.0f);
			glm::mat4 orbitModelMatrix(1.0f);
			glm::mat4 textModelMatrix(1.0f);

			// Circular translation of satellite around corresponding planet
			if (const auto& satelliteParent = dataInput.second.parentInfo)
			{
				const float sinPlanetAngleRot = sin(satelliteParent->angleRot);

				defaultModelMatrix = glm::translate(defaultModelMatrix, glm::vec3(
					preComputations[dataInput.second.parentName].cosCircularTslCst * sinPlanetAngleRot,
					preComputations[dataInput.second.parentName].sinCircularTslCst * sinPlanetAngleRot,
					satelliteParent->dist * cos(satelliteParent->angleRot)));
				orbitModelMatrix = defaultModelMatrix;
			}

			// Orbital tilt (around axis colinear to orbit direction) + Circular translation along the orbit (equidistance to axis normal to orbital plane)
			const float sinAngleRot = sin(angleRot);
			defaultModelMatrix = glm::translate(defaultModelMatrix, glm::vec3(
				preComputations[dataInput.first].cosCircularTslCst * sinAngleRot,
				preComputations[dataInput.first].sinCircularTslCst * sinAngleRot,
				dataInput.second.dist * cos(angleRot)));
			textModelMatrix = defaultModelMatrix;

			// Axis tilt (around axis colinear to orbit direction)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, preComputations[dataInput.first].obliquityInRad, Utils::rightVector);

			// Rotation on itself (around axis normal to orbital plane)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, angleRotItself, Utils::upVector);

			// Draw Saturn rings
			if (dataInput.first == "Saturn")
			{
				defaultShader.Enable();
				defaultShader.setUniformMat4("model", defaultModelMatrix);
				defaultShader.setUniformInt("materialDiffuse", samplerID);
				saturnRings.Render(renderer, samplerID++);
				defaultShader.Disable();
			}

			// Rotation on itself (to have celestial body poles vertical)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, Utils::halfPi, Utils::rightVector);

			if (dataInput.first == "Sun")
			{
				sunShader.Enable();
				sunShader.setUniformInt("materialDiffuse", samplerID);
				sunShader.Disable();
			}
			else
			{
				defaultShader.Enable();
				defaultShader.setUniformInt("materialDiffuse", samplerID);
				defaultShader.Disable();
			}

			// Spherical celestial bodies
			if (const auto& sphere = dataInput.second.sphere)
			{
				if (dataInput.first == "Sun")
				{
					sunShader.Enable();
					sunShader.setUniformMat4("model", defaultModelMatrix);
					sphere->Render(renderer, samplerID++);
					sunShader.Disable();
				}
				else
				{
					defaultShader.Enable();
					defaultShader.setUniformMat4("model", defaultModelMatrix);
					sphere->Render(renderer, samplerID++);
					defaultShader.Disable();
				}
			}





			// Draw billboard (containing current celestial body name) on top of current celestial body mesh/model
			if (window.IsSimuPaused())
			{
				// Orient text to camera position
				const glm::vec3& look = glm::normalize(cameraPosition - glm::vec3(textModelMatrix[3]));
				const glm::vec3& right = glm::cross(camera.GetUp(), look);
				const glm::vec3& up2 = cross(look, right);
				textModelMatrix[0] = glm::vec4(right, 0);
				textModelMatrix[1] = glm::vec4(up2, 0);
				textModelMatrix[2] = glm::vec4(look, 0);

				textShader.Enable();
				textShader.setUniformMat4("model", textModelMatrix);
				textShader.setUniformInt("texSampler", samplerID);
				textShader.setUniformVec3("textColor", Utils::whiteColour);

				if (dataInput.first != "Sun")
				{
					textRenderer.Render(renderer, dataInput.first,
						0.0f, preComputations[dataInput.first].textHeight, preComputations[dataInput.first].textScale, samplerID++);
				}
				else
				{
					textRenderer.Render(renderer, dataInput.first,
						0.0f, preComputations[dataInput.first].sunTextHeight, preComputations[dataInput.first].sunTextScale, samplerID++);
				}

				textShader.Disable();
			}





			// Draw planet orbits
			if (dataInput.first != "Sun")
			{
				orbitModelMatrix = glm::rotate(orbitModelMatrix, preComputations[dataInput.first].orbInclinationInRad, Utils::backVector);

				defaultShader.Enable();
				defaultShader.setUniformMat4("model", orbitModelMatrix);
				defaultShader.setUniformInt("materialDiffuse", samplerID);
				dataInput.second.orbit->Render(renderer, samplerID++);
				defaultShader.Disable();
			}
		}





		// Draw the 2 main belts composed
		instancedModelShader.Enable();
		instancedModelShader.setUniformVec3("viewPos", cameraPosition);
		instancedModelShader.setUniformInt("materialDiffuse", samplerID);
		asteroidBelt.Render(renderer, samplerID++);
		instancedModelShader.setUniformInt("materialDiffuse", samplerID);
		kuiperBelt.Render(renderer, samplerID++);
		instancedModelShader.Disable();





		// Draw Milky Way skybox
		uboMatrices.InitSubData({ {glm::value_ptr(glm::mat4(glm::mat3(viewMatrix))), Utils::mat4v4Size} }, Utils::mat4v4Size);

		skyboxShader.Enable();
		skyboxShader.setUniformInt("texSampler", samplerID);
		renderer.DepthEqual();
		skybox.Render(renderer, samplerID++);
		renderer.DepthLess();
		skyboxShader.Disable();





		window.SwapBuffers();
		window.PollEvents();
	}

	window.FreeUpResources();

	return 0;
}