/////////////////////////////////////
////////// BEGIN MANDATORY //////////
/////////////////////////////////////
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
/////////////////////////////////////
////////// END MANDATORY ////////////
/////////////////////////////////////

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Don't put the "real" path because doesn't work ! Implicitely managed by Visual Studio I suppose...
#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Sphere.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Initial dimensions for the graphics window
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1600;

// Camera object initialization
Camera camera(glm::vec3(0.0f, 0.0f, 200.0f));

// Variables for mouse callback function
bool firstMouseInput = true;
double lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;

// Time related to frames
float deltaTime = 0.0f;
float lastFrame = 0.0f;



/////////////////////////////////////
////////// BEGIN MANDATORY //////////
/////////////////////////////////////
// Check whether a specific key (escape here) is pressed / released 
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Enable camera to move up, down, left and right (designed for AZERTY keyboards with corresponding QWERTY GLFW_KEYs)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Adapt the viewport whenever the window size has been changed (by OS or the user)
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// Make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	// Avoid little jump 
	if (firstMouseInput)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouseInput = false;
	}

	double xoffset = xpos - lastX;
	// Reversed since y-coordinates go from bottom to top
	double yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement((float)xoffset, (float)yoffset, firstMouseInput);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

GLFWwindow* initGLFWWindow(int windowWidth, int windowHeight, const char* windowTitle)
{
	// Initialization of a GLFW window
	glfwInit();

	// Configuration of the GLFW window (1st arg indicates the option to configure, 2nd arg sets the value for this option)
	// Set major and minor version of OpenGL to 3, to ensure others that have not this OpenGL version GLFW failure
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell explicitly GLFW that we want to use core-profile 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creation of a GLFW window of size windth x height, which title will appear in a top bar
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

	return window;
}

void colorationGLFWWindow()
{
	// Set RGB + alpha coefficients for background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Clear the content of the window
	glClear(GL_COLOR_BUFFER_BIT);
}

void CreateTexture(GLuint& textElmtID, int canals, const char * imagePath)
{
	// GENERATION T2O - Generate 1 TO storing its ID within the provided array (graphics memory)
	//glGenTextures(1, &textElmtID);

	// BINDING T2O - Bind TO to the OpenGL TO type we want (here a 2D texture called GL_TEXTURE_2D, hence T2O variable name)
	glBindTexture(GL_TEXTURE_2D, textElmtID);

	// Set the texture wrapping option (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set the texture filtering option (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	// Load image (without alpha channel)
	unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 3);

	// Tell stb_image.h to flip loaded texture's on the y-axis (to see textures applied normal to outward faces directions)
	stbi_set_flip_vertically_on_load(true);

	if (data)
	{
		// Generate texture image on the currently bound TO at the active texture unit
		glTexImage2D(GL_TEXTURE_2D, 0, canals, width, height, 0, canals, GL_UNSIGNED_BYTE, data);

		// Generate all mipmap levels for the previous currently bound TO
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load box texture" << std::endl;

	stbi_image_free(data);
}










int main()
{
	GLFWwindow* window = initGLFWWindow(SCR_WIDTH, SCR_HEIGHT, "My first GLFW window !");

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Tell GLFW that we want the window context to be the main one on the current thread
	glfwMakeContextCurrent(window);

	// Set the framebuffer resize callback of the specified window, called when the framebuffer of the specified window is resized
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Detect if any mouse movement is made and react accordingly
	glfwSetCursorPosCallback(window, mouseCallback);

	// Detect if any mouse whell movement is made and react accordingly
	glfwSetScrollCallback(window, scrollCallback);

	// Make the cursor invisible to the player and allow movements even if cursor is theorically outside the window
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load all OpenGL function pointers (localisations) thanks to GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	/////////////////////////////////////
	////////// END MANDATORY ////////////
	/////////////////////////////////////










	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_DEPTH_CLAMP);


	// Build and compile our shader program (no need text files .txt)
	Shader shaderSphere("SphereShader.vs", "SphereShader.fs");
	//unsigned int shaderID = shaderSphere.ID;

	// GENERATION T2O - Generate 10 TO storing its ID within texTab array (graphics memory)
	GLuint texTab[10];
	glGenTextures(10, texTab);

	// Load texture image files
	CreateTexture(texTab[0], GL_RGB, "../Images/8k_sun.tga");
	CreateTexture(texTab[1], GL_RGB, "../Images/8k_mercury.tga");
	CreateTexture(texTab[2], GL_RGB, "../Images/8k_venus.tga");
	CreateTexture(texTab[3], GL_RGB, "../Images/8k_earth.tga");
	CreateTexture(texTab[4], GL_RGB, "../Images/8k_mars.tga");
	CreateTexture(texTab[5], GL_RGB, "../Images/8k_jupiter.tga");
	CreateTexture(texTab[6], GL_RGB, "../Images/8k_saturn.tga");
	CreateTexture(texTab[7], GL_RGB, "../Images/2k_uranus.tga");
	CreateTexture(texTab[8], GL_RGB, "../Images/2k_neptune.tga");
	CreateTexture(texTab[9], GL_RGB, "../Images/pluto.tga");  

	//// Factors are done to be able to see planets together
	//float factor = 10000.0f;
	//float factor2 = 100000.0f;
	//float million = 1000000.0f;

	//float sun_radius = 696342.0f / factor;
	//std::vector<float> radii;
	//float mercury_radius = 2439.7f;
	//radii.push_back(mercury_radius / factor);
	//float venus_radius = 6051.8f;
	//radii.push_back(venus_radius / factor);
	//float earth_radius = 6371.0f;
	//radii.push_back(earth_radius / factor);
	//float mars_radius = 3389.5f;
	//radii.push_back(mars_radius / factor);
	//float jupiter_radius = 69911.0f;
	//radii.push_back(jupiter_radius / factor);
	//float saturn_radius = 58232.0f;
	//radii.push_back(saturn_radius / factor);
	//float uranus_radius = 25362.0f;
	//radii.push_back(uranus_radius / factor);
	//float neptune_radius = 24622.0f;
	//radii.push_back(neptune_radius / factor);
	//float pluto_radius = 1188.3f;
	//radii.push_back(pluto_radius / factor);

	//glm::vec3 sunPos = glm::vec3(0.0f, 0.0f, 0.0f);
	//std::vector<float> distances;
	//float mercury_distance = sun_radius + 5.70f * million / factor2;
	//distances.push_back(mercury_distance);
	//float venus_distance = mercury_distance + 10.80f * million / factor2;
	//distances.push_back(venus_distance);
	//float earth_distance = venus_distance + 15.00f * million / factor2;
	//distances.push_back(earth_distance);
	//float mars_distance = earth_distance + 22.80f * million / factor2;
	//distances.push_back(mars_distance);
	//float jupiter_distance = mars_distance + 77.90f * million / factor2;
	//distances.push_back(jupiter_distance);
	//float saturn_distance = jupiter_distance + 143.00f * million / factor2;
	//distances.push_back(saturn_distance);
	//float uranus_distance = saturn_distance + 288.00f * million / factor2;
	//distances.push_back(uranus_distance);
	//float neptune_distance = uranus_distance + 450.00f * million / factor2;
	//distances.push_back(neptune_distance);
	//float pluto_distance = neptune_distance + 591.00f * million / factor2;
	//distances.push_back(pluto_distance);

	// Planet diameters in relation to earth (scaled so that earth radius = 1)
	float sun_radius = 109.3f;
	std::vector<float> radii;
	float mercury_radius = 0.383f;
	radii.push_back(mercury_radius);
	float venus_radius = 0.95f;
	radii.push_back(venus_radius);
	float earth_radius = 1.0f;
	radii.push_back(earth_radius);
	float mars_radius = 0.532f;
	radii.push_back(mars_radius);
	float jupiter_radius = 10.97f;
	radii.push_back(jupiter_radius);
	float saturn_radius = 9.14f;
	radii.push_back(saturn_radius);
	float uranus_radius = 3.981f;
	radii.push_back(uranus_radius);
	float neptune_radius = 3.865f;
	radii.push_back(neptune_radius);
	float pluto_radius = 0.186f;
	radii.push_back(pluto_radius);
	//double moon_radius = 0.2724;

	//Planet distances from sun in relation to earth's distance
	glm::vec3 sunPos = glm::vec3(0.0f, 0.0f, 0.0f);
	float factor = 10.0f;
	std::vector<float> distances;
	float mercury_distance = sun_radius + 0.38f * factor;
	distances.push_back(mercury_distance);
	float venus_distance = mercury_distance + 0.72f * factor;
	distances.push_back(venus_distance);
	float earth_distance = venus_distance + 1.0f * factor;
	distances.push_back(earth_distance);
	float mars_distance = earth_distance + 1.52f * factor;
	distances.push_back(mars_distance);
	float jupiter_distance = mars_distance + 5.19f * factor;
	distances.push_back(jupiter_distance);
	float saturn_distance = jupiter_distance + 9.53f * factor;
	distances.push_back(saturn_distance);
	float uranus_distance = saturn_distance + 19.20f * factor;
	distances.push_back(uranus_distance);
	float neptune_distance = uranus_distance + 30.05f * factor;
	distances.push_back(neptune_distance);
	float pluto_distance = neptune_distance + 39.24f * factor;
	distances.push_back(pluto_distance);

	// radius doesn't affect the spherical mesh...
	Sphere * sun = new Sphere(sun_radius);
	Sphere * planets[9];
	for (int i = 0; i < 9; ++i)
	{
		planets[i] = new Sphere(radii[i]);
	}










	/////////////////////////////////////
	////////// BEGIN MANDATORY //////////
	/////////////////////////////////////
	// RENDER LOOP (check at the start of each loop iteration if GLFW has been instructed to close)
	while (!glfwWindowShouldClose(window))
	{
		// Compute delta time in order to offer each player the same experience, by adapting to their different processing powers, drawing much more frames than others per second
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if a specific key is pressed and react accordingly every frame
		processInput(window);

		colorationGLFWWindow();
		///////////////////////////////////////
		//////////// END MANDATORY ////////////
		///////////////////////////////////////



		// Clear the depth buffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate the PROJECTION matrix (since it rarely changes, it's often best practice to set it once outside the main loop)
		// Set far plane to a sufficiently high value to be able to visualize all meshes
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 1000.0f);

		// Calculate the VIEW matrix (simulate a camera circling around the scene)
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();

		// Calculate the MODEL matrix for the sun (with translations applied to center it) 
		glm::mat4 modelSun = glm::mat4(1.0f);
		modelSun = glm::translate(modelSun, sunPos);
		modelSun = glm::rotate(modelSun, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		// Activate the shader to initialize projection, view then model mat4 inside
		shaderSphere.use();
		//glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, &projection[0][0]);
		//glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, &view[0][0]);
		//glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &modelSun[0][0]);
		shaderSphere.setMat4("projection", projection);
		shaderSphere.setMat4("view", view);
		shaderSphere.setMat4("model", modelSun);

		// Activate the texture unit before binding texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texTab[0]);

		//glUniform1i(glGetUniformLocation(shaderID, "texSphere"), 0);
		shaderSphere.setInt("texSphere", 0);

		sun->Draw();





		// Render 10 cubes
		for (unsigned int i = 0; i < 9; i++)
		{
			// Calculate the MODEL matrix for each cube (with translations applied to see them all) 
			glm::mat4 modelPlanet = glm::mat4(1.0f);

			//float angle = 0.006f * i  * speed;
			float angleSat = (float)(glfwGetTime() * (i+1));
			float argTrigo = glm::pi<float>() * 2 * angleSat / 360;

			//modelPlanet = glm::translate(modelPlanet, sunPos + glm::vec3(0.0f, 0.0f, -50.0f * i));

			// Position according to distance sun - planet
			modelPlanet = glm::translate(modelPlanet, glm::vec3(distances[i] * sin(argTrigo), 0.0f, distances[i] * cos(argTrigo)));
			// Rotation (permanent since the beginning) on itself to have texture seen horizontaly
			modelPlanet = glm::rotate(modelPlanet, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			// Rotation on itself = PROGRADE MOTION (not retrograde)
			modelPlanet = glm::rotate(modelPlanet, angleSat, glm::vec3(0.0f, 0.0f, 1.0f));
			//// Rotation around the sun = ORBITAL MOTION
			//modelPlanet = glm::rotate(modelPlanet, (float)(glm::radians(60.0f) * glfwGetTime()), glm::vec3(0.0f, 0.1f, 0.0f));

			shaderSphere.use();
			shaderSphere.setMat4("model", modelPlanet);

			// Activate the texture unit before binding texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texTab[i + 1]);

			shaderSphere.setInt("texSphere", 0);

			planets[i]->Draw();
		}



		///////////////////////////////////////
		//////////// BEGIN MANDATORY //////////
		///////////////////////////////////////
		//// Swap font and back buffers (we sent to the screen the updated buffer)
		glfwSwapBuffers(window);

		// Check if any events are triggered, updates the window states and call the corresponding functions
		glfwPollEvents();
	}

	// Clean up properly all previous allocated GLFW resources
	glfwTerminate();

	return 0;
	/////////////////////////////////////
	////////// END MANDATORY ////////////
	/////////////////////////////////////
}


