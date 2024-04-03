#include <iostream>
#include <glad.h>
#include <glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

#include "Belt.h"
#include "Camera.h"
#include "Data.h"
#include "Model.h"
#include "Orbit.h"
#include "Renderer.h"
#include "Shader.h"
#include "Skybox.h"
#include "Sphere.h"
#include "Text.h"
#include "Window.h"





int main()
{
	constexpr unsigned int SCR_WIDTH = 1000;
	constexpr unsigned int SCR_HEIGHT = 1000;
	Window window(SCR_WIDTH, SCR_HEIGHT, "Solar System Simulation");

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
	Shader textShader("TextShader.vs", "TextShader.fs");
	Shader skyboxShader("SkyboxShader.vs", "SkyboxShader.fs");
	Shader instancedModelShader("InstancedModelShader.vs", "InstancedModelShader.fs");

	// Create Milky Way skybox
	Skybox skybox = Skybox("../Textures/MilkyWay/stars.dds");

	// Create text characters
	Text text;

	// Load models (meshes with textures applied)
	Model saturnRings("../Models/SaturnRings.obj");
	Model asteroid("../Models/Asteroid.obj");
	Model ice("../Models/Ice.obj");
	Model deimos("../Models/Deimos.obj");
	Model phobos("../Models/Phobos.obj");





	// Initialise all celestial bodies and their respective orbit (applying celectial body textures to have some colour consistency)
	LoadData();
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		if (it->first == "Sun")
		{
			it->second.sphere = new Sphere(it->second.texturePath, it->second.radius * 0.5f);
		}
		else if (it->first == "Deimos" || it->first == "Phobos")
		{
			// @todo - Have a reference of the non-spherical satellite (of type Model) in the data structure instead?
			it->second.orbit = new Orbit(it->second.texturePath, it->second.dist);
		}
		else
		{
			it->second.sphere = new Sphere(it->second.texturePath, it->second.radius);
			it->second.orbit = new Orbit(it->second.texturePath, it->second.dist);
		}
	}

	// Do some instancing to build the main Solar Systems rock belts
	Belt asteroidBelt	{ asteroid, 5000,  10, data["Mars"].dist * 1.1f,	2.75f * DIST_SCALE_FACTOR / 2.5f };
	Belt kuiperBelt		{ ice,		20000, 20, data["Neptune"].dist * 1.4f, 30.05f * DIST_SCALE_FACTOR };





	// Create renderer
	Renderer renderer;
	renderer.DepthTest();

	// RENDER LOOP (iterating every frame)
	while (window.DoNotClose())
	{
		window.UpdateFrames();
		window.ProcessInput(camera);

		renderer.Clear();
		renderer.Blend();

		// Simulate a zoom - set far plane variable to a sufficiently high value
		const glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(SCR_WIDTH / SCR_HEIGHT), 0.1f, 1000.0f);

		// Simulate a camera circling around the scene
		const glm::mat4 viewMatrix = camera.GetViewMatrix();

		// Texture sampler ID (one for each object) 
		unsigned int samplerID = 0;

		defaultShader.Enable();
		defaultShader.setUniformMat4("projection", projectionMatrix);
		defaultShader.setUniformMat4("view", viewMatrix);
		defaultShader.setUniformVec3("material.specular", 0.0f, 0.0f, 0.0f);
		defaultShader.setUniformFloat("material.shininess", 64.0f);
		defaultShader.setUniformVec3("light.position", 0.0f, 0.0f, 0.0f);
		defaultShader.setUniformVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		defaultShader.setUniformVec3("light.diffuse", 0.95f, 0.95f, 0.95f);
		defaultShader.setUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);
		defaultShader.setUniformFloat("light.isBlinn", false);
		defaultShader.setUniformFloat("light.constant", 1.0f);
		defaultShader.setUniformFloat("light.linear", 0.0007f);
		defaultShader.setUniformFloat("light.quadratic", 0.000002f);
		defaultShader.setUniformVec3("viewPos", camera.GetPosition());

		// Draw celestial bodies, their orbits and their motion
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			float angleRotInRadians = 0.0f;
			float angleRotItselfInRadians = 0.0f;

			if (it->first != "Sun")
			{
				// Angle of rotation around the sun (resp. planet) for planets (resp. moons) per frame
				angleRotInRadians = 2.0f * glm::pi<float>() * window.GetCurrentFrame() / it->second.orbPeriod * window.GetSimuSpeedFactor();
				if (it->second.planetInfo == nullptr)
				{
					it->second.angleRotInRadians = angleRotInRadians;
				}

				// Angle of rotation of the celestial body around itself per frame
				angleRotItselfInRadians = 2.0f * glm::pi<float>() * window.GetCurrentFrame() * it->second.rotPeriod * window.GetSimuSpeedFactor();
			
				defaultShader.Enable();
				defaultShader.setUniformBool("isSun", false);
			}
			else
			{
				defaultShader.Enable();
				defaultShader.setUniformBool("isSun", true);
			}

			// Simulate movements that affects the current celestial body
			glm::mat4 defaultModelMatrix = glm::mat4(1.0f);
			glm::mat4 orbitModelMatrix = glm::mat4(1.0f);
			glm::mat4 textModelMatrix = glm::mat4(1.0f);

			// Circular translation around corresponding planet (condition applies for satellites only)
			if (it->second.planetInfo != nullptr)
			{
				const float satteliteOrbInclinationInRadians = glm::radians(it->second.planetInfo->orbInclination);
				defaultModelMatrix = glm::translate(defaultModelMatrix, glm::vec3(
					it->second.planetInfo->dist * cos(satteliteOrbInclinationInRadians) * sin(it->second.planetInfo->angleRotInRadians),
					it->second.planetInfo->dist * sin(satteliteOrbInclinationInRadians) * sin(it->second.planetInfo->angleRotInRadians),
					it->second.planetInfo->dist * cos(it->second.planetInfo->angleRotInRadians)));
				orbitModelMatrix = defaultModelMatrix;
			}

			// Orbital tilt (around axis colinear to orbit direction) + Circular translation along the orbit (equidistance to axis normal to orbital plane)
			const float planetOrbInclinationInRadians = glm::radians(it->second.orbInclination);
			defaultModelMatrix = glm::translate(defaultModelMatrix, glm::vec3(
				it->second.dist * cos(planetOrbInclinationInRadians) * sin(angleRotInRadians),
				it->second.dist * sin(planetOrbInclinationInRadians) * sin(angleRotInRadians),
				it->second.dist * cos(angleRotInRadians)));
			textModelMatrix = defaultModelMatrix;

			// Axis tilt (around axis colinear to orbit direction)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, glm::radians(it->second.obliquity), glm::vec3(1.0f, 0.0f, 0.0f));

			// Rotation on itself (around axis normal to orbital plane)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, angleRotItselfInRadians, glm::vec3(0.0f, 1.0f, 0.0f));

			// Draw Saturn rings
			if (it->first == "Saturn")
			{
				defaultShader.Enable();
				defaultShader.setUniformBool("isSun", false);
				defaultShader.setUniformMat4("model", defaultModelMatrix);
				defaultShader.setUniformInt("material.diffuse", samplerID);
				saturnRings.Render(renderer, samplerID++);
			}

			// Rotation on itself (to have celestial body poles vertical)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			defaultShader.Enable();
			defaultShader.setUniformInt("material.diffuse", samplerID);

			if (it->second.sphere)
			{
				defaultShader.Enable();
				defaultShader.setUniformMat4("model", defaultModelMatrix);

				it->second.sphere->Render(renderer, samplerID++);
			}
			// Non-spherical celestial bodies
			else
			{
				defaultModelMatrix = glm::scale(defaultModelMatrix, glm::vec3(0.025f * it->second.planetInfo->radius));

				defaultShader.Enable();
				defaultShader.setUniformMat4("model", defaultModelMatrix);

				if (it->first == "Deimos")
				{
					deimos.Render(renderer, samplerID++);
				}
				else if (it->first == "Phobos")
				{
					phobos.Render(renderer, samplerID++);
				}
			}





			// Draw billboard (containing current celestial body name) on top of current celestial body mesh/model
			if (window.IsSimuPaused())
			{
				// Orient text to camera position
				const glm::vec3 look = glm::normalize(camera.GetPosition() - glm::vec3(textModelMatrix[3]));
				const glm::vec3 right = glm::cross(camera.GetUp(), look);
				const glm::vec3 up2 = cross(look, right);
				textModelMatrix[0] = glm::vec4(right, 0);
				textModelMatrix[1] = glm::vec4(up2, 0);
				textModelMatrix[2] = glm::vec4(look, 0);

				textShader.Enable();
				textShader.setUniformMat4("projection", projectionMatrix);
				textShader.setUniformMat4("view", viewMatrix);
				textShader.setUniformMat4("model", textModelMatrix);
				textShader.setUniformInt("texSampler", samplerID);
				textShader.setUniformVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f));

				if (it->first == "Deimos" || it->first == "Phobos")
				{
					// @todo - Get the bounding box size of models from Mesh class?
					text.Render(renderer, it->first, 0.0f, it->second.planetInfo->radius * 0.5f, it->second.planetInfo->radius * 0.002f, samplerID++);
				}
				else if (it->first != "Sun")
				{
					text.Render(renderer, it->first, 0.0f, it->second.radius * 1.25f, it->second.radius * 0.01f, samplerID++);
				}
				else
				{
					text.Render(renderer, it->first, 0.0f, it->second.radius * 1.25f * 0.5f, it->second.radius * 0.003f, samplerID++);
				}
			}





			// Draw planet orbits
			if (it->first != "Sun")
			{
				orbitModelMatrix = glm::rotate(orbitModelMatrix, glm::radians(it->second.orbInclination), glm::vec3(0.0f, 0.0f, 1.0f));

				defaultShader.Enable();
				defaultShader.setUniformMat4("model", orbitModelMatrix);
				defaultShader.setUniformInt("material.diffuse", samplerID);
				it->second.orbit->Render(renderer, samplerID++);
			}
		}





		// Draw the 2 main belts composed
		instancedModelShader.Enable();
		instancedModelShader.setUniformMat4("projection", projectionMatrix);
		instancedModelShader.setUniformMat4("view", viewMatrix);
		instancedModelShader.setUniformVec3("material.specular", 0.0f, 0.0f, 0.0f);
		instancedModelShader.setUniformFloat("material.shininess", 64.0f);
		instancedModelShader.setUniformVec3("light.position", 0.0f, 0.0f, 0.0f);
		instancedModelShader.setUniformVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		instancedModelShader.setUniformVec3("light.diffuse", 0.95f, 0.95f, 0.95f);
		instancedModelShader.setUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);
		instancedModelShader.setUniformFloat("light.isBlinn", false);
		instancedModelShader.setUniformFloat("light.constant", 1.0f);
		instancedModelShader.setUniformFloat("light.linear", 0.0007f);
		instancedModelShader.setUniformFloat("light.quadratic", 0.000002f);
		instancedModelShader.setUniformVec3("viewPos", camera.GetPosition());
		instancedModelShader.setUniformInt("material.diffuse", samplerID);
		asteroidBelt.Render(renderer, samplerID++);

		instancedModelShader.Enable();
		instancedModelShader.setUniformInt("material.diffuse", samplerID);
		kuiperBelt.Render(renderer, samplerID++);





		// Draw Milky Way skybox
		renderer.DepthEqual();
		skyboxShader.Enable();
		skyboxShader.setUniformMat4("projection", projectionMatrix);
		skyboxShader.setUniformMat4("view", glm::mat4(glm::mat3(viewMatrix)));
		skyboxShader.setUniformInt("texSampler", samplerID);
		skybox.Render(renderer, samplerID++);
		renderer.DepthLess();





		window.SwapBuffers();
		window.PollEvents();
	}

	window.FreeUpResources();

	return 0;
}