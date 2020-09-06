
#include "Skybox.h"
#include "Text.h"
#include "UX.h"
#include "Belt.h"
#include "Data.h"
#include "Renderer.h"



int main()
{
	GLFWwindow* window = initGLFWWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Solar System Simulation");
	if (window == NULL)
	{
		std::cout << "ERROR::GLFW: Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Tell GLFW that we want the window context to be the main one on the current thread
	glfwMakeContextCurrent(window);
	if (glfwGetCurrentContext() == NULL)
	{
		std::cout << "ERROR::GLFW: Failed to get current context : OpenGL functions will not work correctly" << std::endl;
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
		std::cout << "ERROR::GLAD: Failed to initialize GLAD" << std::endl;
		return -1;
	}





	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_DEPTH_CLAMP);

	// Build and compile shader programs
	ShaderProgram textShader("TextShader.vs", "TextShader.fs");
	ShaderProgram saturnRingsShader("SaturnRingsShader.vs", "SaturnRingsShader.fs");
	ShaderProgram asteroidShader("AsteroidShader.vs", "AsteroidShader.fs");
	ShaderProgram skyboxShader("SkyboxShader.vs", "SkyboxShader.fs");
	ShaderProgram sphereShader("SphereShader.vs", "SphereShader.fs");



	// Create text characters
	Text text;

	// Load models (meshes with textures applied)
	Model saturnRings("../Models/SaturnRings/SaturnRingsTextured.obj");
	Model asteroid("../Models/Asteroid/AsteroidTextured.obj");

	// Create Milky Way skybox
	Skybox skybox = Skybox("../Textures/MilkyWay/stars.dds");

	// Initialize all celestial bodies and their respective orbit
	LoadData();
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		if (it->first == "Sun")
			it->second.sphere = new Sphere(it->second.texturePath, it->second.radius * 0.5f);
		else
		{
			it->second.sphere = new Sphere(it->second.texturePath, it->second.radius);
			it->second.orbit = new Orbit(texturePaths[0], it->second.dist);
		}
	}

	// Do some instancing to build the main Solar Systems rock belts
	Belt asteroidBelt { asteroid, 5000, 10, data["Mars"].dist * 1.1f, 2.75f * DIST_SCALE_FACTOR / 2.5f };
	//Belt kuiperBelt { asteroid, 20000, 20, data["Neptune"].dist * 1.4f, 30.05f * DIST_SCALE_FACTOR };

	Renderer renderer;





	// RENDER LOOP (check at the start of each loop iteration if GLFW has been instructed to close every frame)
	while (!glfwWindowShouldClose(window))
	{
		if (!paused)
		{
			// Time elapsed since GLFW initialisation [considered as a dimensionless chrono, but in seconds in reality]
			currentFrame = (float)glfwGetTime();

			// Compute delta time in order to reduce differences between computer processing powers
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
		}

		// Check if a specific key is pressed and react accordingly
		processInput(window);

		renderer.Clear();
		renderer.Blend();





		// Calculate the PROJECTION matrix (simulate a zoom - set far plane variable to a sufficiently high value)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 1000.0f);

		// Calculate the VIEW matrix (simulate a camera circling around the scene)
		glm::mat4 view = camera.GetViewMatrix();

		// Calculate the MODEL matrices (simulate movements that affects the current celestial body)
		glm::mat4 modelSphere;
		glm::mat4 modelText;
		glm::mat4 modelOrbit;

		// Texture sampler ID (one for each object) 
		unsigned int samplerID = 0;



		// Activate the shaders before initializing uniforms
		sphereShader.Enable();
		sphereShader.setMat4("projection", projection);
		sphereShader.setMat4("view", view);
		sphereShader.setVec3("material.specular", 0.0f, 0.0f, 0.0f);
		sphereShader.setFloat("material.shininess", 64.0f);
		sphereShader.setVec3("light.position", 0.0f, 0.0f, 0.0f);
		sphereShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		sphereShader.setVec3("light.diffuse", 0.95f, 0.95f, 0.95f);
		sphereShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		sphereShader.setFloat("light.constant", 1.0f);
		sphereShader.setFloat("light.linear", 0.0007f);
		sphereShader.setFloat("light.quadratic", 0.000002f);
		sphereShader.setVec3("viewPos", camera.Position);

		textShader.Enable();
		textShader.setMat4("projection", projection);
		textShader.setMat4("view", view);





		float angleRot, angleRotItself;

		// Draw celestial bodies, their orbits and their motion
		for (auto it = data.begin(); it != data.end(); ++it)
		{		
			if (it->first != "Sun")
			{
				// Angle of rotation around the sun (resp. planet) for planets (resp. moons) per frame
				angleRot = 2.0f * glm::pi<float>() * currentFrame / it->second.orbPeriod * ACCELERATION;
				if (it->second.planet == nullptr)
					it->second.angleRot = angleRot;

				// Angle of rotation of the celestial body around itself per frame
				angleRotItself = 2.0f * glm::pi<float>() * currentFrame * it->second.rotPeriod * ACCELERATION;
			}
			else
			{
				// Sun rotation too fast to have a cool look
				angleRot = 0.0f;
				angleRotItself = 0.0f;
			}



			modelSphere = glm::mat4(1.0f);
			modelText = glm::mat4(1.0f);
			modelOrbit = glm::mat4(1.0f);

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
				saturnRingsShader.setMat4("projection", projection);
				saturnRingsShader.setMat4("view", view);
				saturnRingsShader.setMat4("model", modelSphere);
				saturnRingsShader.setInt("material.diffuse", samplerID);
				saturnRingsShader.setVec3("material.specular", 0.0f, 0.0f, 0.0f);
				saturnRingsShader.setFloat("material.shininess", 64.0f);
				saturnRingsShader.setVec3("light.position", 0.0f, 0.0f, 0.0f);
				saturnRingsShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
				saturnRingsShader.setVec3("light.diffuse", 0.95f, 0.95f, 0.95f);
				saturnRingsShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
				saturnRingsShader.setFloat("light.constant", 1.0f);
				saturnRingsShader.setFloat("light.linear", 0.0007f);
				saturnRingsShader.setFloat("light.quadratic", 0.000002f);
				saturnRingsShader.setVec3("viewPos", camera.Position);
				saturnRings.Render(renderer, samplerID);
				++samplerID;

				//for (size_t texIndex = 0; texIndex < texRef.size(); ++texIndex)
				//{
				//	texRef[texIndex].Enable(samplerID);
				//	saturnRings.Draw(saturnRingsShader);

				//	++samplerID;
				//}
			}

			// Rotation on itself (to have spheres poles vertical)
			modelSphere = glm::rotate(modelSphere, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
																						
			sphereShader.Enable();
			sphereShader.setMat4("model", modelSphere);
			if (it->first != "Sun")
				sphereShader.setBool("isSun", false);
			else
				sphereShader.setBool("isSun", true);
			sphereShader.setInt("material.diffuse", samplerID);
			it->second.sphere->Render(renderer, samplerID);
			++samplerID;



			// Draw billboard (containing current celestial body name) on top of current celestial body
			if (paused)
			{
				// Orient text to camera position
				glm::vec3 look = glm::normalize(camera.Position - glm::vec3(modelText[3]));
				glm::vec3 right = glm::cross(camera.Up, look);
				glm::vec3 up2 = cross(look, right);
				modelText[0] = glm::vec4(right, 0);
				modelText[1] = glm::vec4(up2, 0);
				modelText[2] = glm::vec4(look, 0);

				textShader.Enable();
				textShader.setMat4("model", modelText);			
				textShader.setInt("texSampler", samplerID);
				textShader.setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f));						
				if(it->first != "Sun")
					text.Render(renderer, it->first, 0.0f, it->second.radius * 1.25f, it->second.radius * 0.01f, samplerID);
				else
					text.Render(renderer, it->first, 0.0f, it->second.radius * 1.25f * 0.5f, it->second.radius * 0.003f, samplerID);
				++samplerID;
			}



			// Draw planet orbites
			if (it->first != "Sun")
			{			
				modelOrbit = glm::rotate(modelOrbit, glm::radians(it->second.orbInclination), glm::vec3(0.0f, 0.0f, 1.0f));

				sphereShader.Enable();
				sphereShader.setMat4("model", modelOrbit);
				sphereShader.setInt("material.diffuse", samplerID);
				it->second.orbit->Render(renderer, samplerID);
				++samplerID;
			}
		}



		// Draw the 2 main belts composed of asteroids
		asteroidShader.Enable();
		asteroidShader.setMat4("projection", projection);
		asteroidShader.setMat4("view", view);
		asteroidShader.setInt("material.diffuse", samplerID);
		asteroidShader.setVec3("material.specular", 0.0f, 0.0f, 0.0f);
		asteroidShader.setFloat("material.shininess", 64.0f);
		asteroidShader.setVec3("light.position", 0.0f, 0.0f, 0.0f);
		asteroidShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		asteroidShader.setVec3("light.diffuse", 0.95f, 0.95f, 0.95f);
		asteroidShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		asteroidShader.setFloat("light.constant", 1.0f);
		asteroidShader.setFloat("light.linear", 0.0007f);
		asteroidShader.setFloat("light.quadratic", 0.000002f);
		asteroidShader.setVec3("viewPos", camera.Position);
		asteroidBelt.Render(renderer, samplerID);
		//kuiperBelt.Render(renderer, samplerID);
		++samplerID;

		//for (size_t texIndex = 0; texIndex < texRef.size(); ++texIndex)
		//{
		//	texRef[texIndex].Enable(samplerID);
		//	asteroid.Draw(asteroidShader);

		//	++samplerID;
		//}



		// Draw Milky Way skybox
		renderer.DepthEqual();
		skyboxShader.Enable();
		skyboxShader.setMat4("projection", projection);
		skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
		skyboxShader.setInt("texSampler", samplerID);
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