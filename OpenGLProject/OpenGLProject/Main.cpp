#include "Belt.h"
#include "Data.h"
#include "Renderer.h"
#include "Skybox.h"
#include "Text.h"
#include "UX.h"



int main()
{
	GLFWwindow* window = initGLFWWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Solar System Simulation");
	if (window == nullptr)
	{
		std::cout << "ERROR::GLFW - Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Tell GLFW that we want the window context to be the main one on the current thread
	glfwMakeContextCurrent(window);
	if (glfwGetCurrentContext() == nullptr)
	{
		std::cout << "ERROR::GLFW - Failed to get current context : OpenGL functions will not work correctly" << std::endl;
		return -1;
	}

	// Set the framebuffer resize callback of the specified window, called when the framebuffer of the specified window is resized
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Detect if any mouse movement is made and react accordingly
	glfwSetCursorPosCallback(window, mouseCallback);

	// Detect if any mouse whell movement is made and react accordingly
	glfwSetScrollCallback(window, scrollCallback);

	// Make the cursor invisible to the player and allow movements even if cursor is theorically outside the window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load all OpenGL function pointers (localisations) thanks to GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD - Failed to initialise GLAD" << std::endl;
		return -1;
	}

	// Build and compile shader programs
	Shader textShader("TextShader.vs", "TextShader.fs");
	Shader saturnRingsShader("SaturnRingsShader.vs", "SaturnRingsShader.fs");
	Shader asteroidShader("AsteroidShader.vs", "AsteroidShader.fs");
	Shader skyboxShader("SkyboxShader.vs", "SkyboxShader.fs");
	Shader sphereShader("SphereShader.vs", "SphereShader.fs");



	// Create text characters
	Text text;

	// Load models (meshes with textures applied)
	Model saturnRings("../Models/SaturnRings.obj");
	Model asteroid("../Models/Asteroid.obj");
	Model ice("../Models/Ice.obj");
	//Model deimos("../Models/Deimos.obj");
	//Model phobos("../Models/Phobos.obj");

	// Create Milky Way skybox
	Skybox skybox = Skybox("../Textures/MilkyWay/stars.dds");

	// Initialise all celestial bodies and their respective orbit
	LoadData();
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		if (it->first == "Sun")
		{
			it->second.sphere = new Sphere(it->second.texturePath, it->second.radius * 0.5f);
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





	// RENDER LOOP (check at the start of each loop iteration if GLFW has been instructed to close every frame)
	while (!glfwWindowShouldClose(window))
	{
		if (!paused)
		{
			// Time elapsed since GLFW initialisation [considered as a dimensionless chrono, but in seconds in reality]
			currentFrame = static_cast<float>(glfwGetTime());

			// Compute delta time in order to reduce differences between computer processing powers
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
		}

		// Check if a specific key is pressed and react accordingly
		processInput(window);

		renderer.Clear();
		renderer.Blend();





		// Calculate the PROJECTION matrix (simulate a zoom - set far plane variable to a sufficiently high value)
		const glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH / SCR_HEIGHT), 0.1f, 1000.0f);

		// Calculate the VIEW matrix (simulate a camera circling around the scene)
		const glm::mat4 view = camera.GetViewMatrix();

		// Texture sampler ID (one for each object) 
		unsigned int samplerID = 0;



		// Activate the shaders before initializing uniforms
		sphereShader.Enable();
		sphereShader.setUniformMat4("projection", projection);
		sphereShader.setUniformMat4("view", view);
		sphereShader.setUniformVec3("material.specular", 0.0f, 0.0f, 0.0f);
		sphereShader.setUniformFloat("material.shininess", 64.0f);
		sphereShader.setUniformVec3("light.position", 0.0f, 0.0f, 0.0f);
		sphereShader.setUniformVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		sphereShader.setUniformVec3("light.diffuse", 0.95f, 0.95f, 0.95f);
		sphereShader.setUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);
		sphereShader.setUniformFloat("light.isBlinn", false);
		sphereShader.setUniformFloat("light.constant", 1.0f);
		sphereShader.setUniformFloat("light.linear", 0.0007f);
		sphereShader.setUniformFloat("light.quadratic", 0.000002f);
		sphereShader.setUniformVec3("viewPos", camera.Position);

		textShader.Enable();
		textShader.setUniformMat4("projection", projection);
		textShader.setUniformMat4("view", view);



		// Draw celestial bodies, their orbits and their motion
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			float angleRot = 0.0f;
			float angleRotItself = 0.0f;

			if (it->first != "Sun")
			{
				// Angle of rotation around the sun (resp. planet) for planets (resp. moons) per frame
				angleRot = 2.0f * glm::pi<float>() * currentFrame / it->second.orbPeriod * ACCELERATION;
				if (it->second.planet == nullptr)
				{
					it->second.angleRot = angleRot;
				}

				// Angle of rotation of the celestial body around itself per frame
				angleRotItself = 2.0f * glm::pi<float>() * currentFrame * it->second.rotPeriod * ACCELERATION;
			}

			// Calculate the MODEL matrices (simulate movements that affects the current celestial body)
			glm::mat4 modelSphere = glm::mat4(1.0f);
			glm::mat4 modelText = glm::mat4(1.0f);
			glm::mat4 modelOrbit = glm::mat4(1.0f);

			// Circular translation around corresponding planet (condition applies for moons only)
			if (it->second.planet != nullptr)
			{
				modelSphere = glm::translate(modelSphere, glm::vec3(it->second.planet->dist * cos(glm::radians(it->second.planet->orbInclination)) * sin(it->second.planet->angleRot), it->second.planet->dist * sin(glm::radians(it->second.planet->orbInclination)) * sin(it->second.planet->angleRot), it->second.planet->dist * cos(it->second.planet->angleRot)));
				modelOrbit = modelSphere;
			}
			// Orbital tilt (around axis colinear to orbit direction) + Circular translation along the orbit (equidistance to axis normal to orbital plane)
			modelSphere = glm::translate(modelSphere, glm::vec3(it->second.dist * cos(glm::radians(it->second.orbInclination)) * sin(angleRot), it->second.dist * sin(glm::radians(it->second.orbInclination)) * sin(angleRot), it->second.dist * cos(angleRot)));
			modelText = modelSphere;
			// Axis tilt (around axis colinear to orbit direction)
			modelSphere = glm::rotate(modelSphere, glm::radians(it->second.obliquity), glm::vec3(1.0f, 0.0f, 0.0f));
			// Rotation on itself (around axis normal to orbital plane)
			modelSphere = glm::rotate(modelSphere, angleRotItself, glm::vec3(0.0f, 1.0f, 0.0f));

			// Draw Saturn rings
			if (it->first == "Saturn")
			{
				saturnRingsShader.Enable();
				saturnRingsShader.setUniformMat4("projection", projection);
				saturnRingsShader.setUniformMat4("view", view);
				saturnRingsShader.setUniformMat4("model", modelSphere);
				saturnRingsShader.setUniformInt("material.diffuse", samplerID);
				saturnRingsShader.setUniformVec3("material.specular", 0.0f, 0.0f, 0.0f);
				saturnRingsShader.setUniformFloat("material.shininess", 64.0f);
				saturnRingsShader.setUniformVec3("light.position", 0.0f, 0.0f, 0.0f);
				saturnRingsShader.setUniformVec3("light.ambient", 0.2f, 0.2f, 0.2f);
				saturnRingsShader.setUniformVec3("light.diffuse", 0.95f, 0.95f, 0.95f);
				saturnRingsShader.setUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);
				saturnRingsShader.setUniformFloat("light.isBlinn", false);
				saturnRingsShader.setUniformFloat("light.constant", 1.0f);
				saturnRingsShader.setUniformFloat("light.linear", 0.0007f);
				saturnRingsShader.setUniformFloat("light.quadratic", 0.000002f);
				saturnRingsShader.setUniformVec3("viewPos", camera.Position);
				saturnRings.Render(renderer, samplerID);
				++samplerID;
			}

			// Rotation on itself (to have spheres poles vertical)
			modelSphere = glm::rotate(modelSphere, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			sphereShader.Enable();
			sphereShader.setUniformMat4("model", modelSphere);
			if (it->first != "Sun")
			{
				sphereShader.setUniformBool("isSun", false);
			}
			else
			{
				sphereShader.setUniformBool("isSun", true);
			}
			sphereShader.setUniformInt("material.diffuse", samplerID);
			it->second.sphere->Render(renderer, samplerID);
			++samplerID;



			// Draw billboard (containing current celestial body name) on top of current celestial body
			if (paused)
			{
				// Orient text to camera position
				const glm::vec3 look = glm::normalize(camera.Position - glm::vec3(modelText[3]));
				const glm::vec3 right = glm::cross(camera.Up, look);
				const glm::vec3 up2 = cross(look, right);
				modelText[0] = glm::vec4(right, 0);
				modelText[1] = glm::vec4(up2, 0);
				modelText[2] = glm::vec4(look, 0);

				textShader.Enable();
				textShader.setUniformMat4("model", modelText);
				textShader.setUniformInt("texSampler", samplerID);
				textShader.setUniformVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f));
				if (it->first != "Sun")
				{
					text.Render(renderer, it->first, 0.0f, it->second.radius * 1.25f, it->second.radius * 0.01f, samplerID);
				}
				else
				{
					text.Render(renderer, it->first, 0.0f, it->second.radius * 1.25f * 0.5f, it->second.radius * 0.003f, samplerID);
				}
				++samplerID;
			}



			// Draw planet orbits
			if (it->first != "Sun")
			{
				modelOrbit = glm::rotate(modelOrbit, glm::radians(it->second.orbInclination), glm::vec3(0.0f, 0.0f, 1.0f));

				sphereShader.Enable();
				sphereShader.setUniformMat4("model", modelOrbit);
				sphereShader.setUniformInt("material.diffuse", samplerID);
				it->second.orbit->Render(renderer, samplerID);
				++samplerID;
			}
		}



		// Draw the 2 main belts composed of asteroids
		asteroidShader.Enable();
		asteroidShader.setUniformMat4("projection", projection);
		asteroidShader.setUniformMat4("view", view);
		asteroidShader.setUniformVec3("material.specular", 0.0f, 0.0f, 0.0f);
		asteroidShader.setUniformFloat("material.shininess", 64.0f);
		asteroidShader.setUniformVec3("light.position", 0.0f, 0.0f, 0.0f);
		asteroidShader.setUniformVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		asteroidShader.setUniformVec3("light.diffuse", 0.95f, 0.95f, 0.95f);
		asteroidShader.setUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);
		asteroidShader.setUniformFloat("light.isBlinn", false);
		asteroidShader.setUniformFloat("light.constant", 1.0f);
		asteroidShader.setUniformFloat("light.linear", 0.0007f);
		asteroidShader.setUniformFloat("light.quadratic", 0.000002f);
		asteroidShader.setUniformVec3("viewPos", camera.Position);
		asteroidShader.setUniformInt("material.diffuse", samplerID);
		asteroidBelt.Render(renderer, samplerID);
		++samplerID;
		asteroidShader.setUniformInt("material.diffuse", samplerID);
		kuiperBelt.Render(renderer, samplerID);
		++samplerID;



		// Draw Milky Way skybox
		renderer.DepthEqual();
		skyboxShader.Enable();
		skyboxShader.setUniformMat4("projection", projection);
		skyboxShader.setUniformMat4("view", glm::mat4(glm::mat3(view)));
		skyboxShader.setUniformInt("texSampler", samplerID);
		skybox.Render(renderer, samplerID);
		renderer.DepthLess();





		// Swap font and back buffers (we sent to the screen the updated buffer)
		glfwSwapBuffers(window);

		// Check if any events are triggered, updates the window states and call the corresponding functions
		glfwPollEvents();
	}

	// Clean up properly all previous allocated GLFW resources
	glfwTerminate();

	return 0;
}