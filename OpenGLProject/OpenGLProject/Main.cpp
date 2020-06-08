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
#include "Sphere.h"
#include "Skybox.h"
#include "Orbit.h"

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

void CreateCubemap(GLuint& textElmtID)
{
	std::vector<std::string> faces
	{
		"../Images/skybox/milky_way_right.jpg",				// right
		"../Images/skybox/milky_way_right.jpg",				// -> left
		"../Images/skybox/stars.jpg",						// top 
		"../Images/skybox/stars.jpg",						// bottom
		"../Images/skybox/milky_way_left.jpg",				// back
		"../Images/skybox/milky_way_left.jpg"				// -> front
	};

	glBindTexture(GL_TEXTURE_CUBE_MAP, textElmtID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	int width, height, nrChannels;
	unsigned char *data;

	for (GLuint i = 0; i < faces.size(); i++)
	{
		data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		//stbi_set_flip_vertically_on_load(true);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load one of the skybox texture" << std::endl;
			stbi_image_free(data);
		}
	}


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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	Shader shaderSkybox("CubeShader.vs", "CubeShader.fs");

	// GENERATION T2O - Generate 10 TO storing its ID within texTab array (graphics memory)
	GLuint texTab[19];
	glGenTextures(19, texTab);

	// Load texture image files
	CreateTexture(texTab[0], GL_RGB, "../Images/8k_sun.jpg");

	CreateTexture(texTab[1], GL_RGB, "../Images/8k_mercury.jpg");
	CreateTexture(texTab[2], GL_RGB, "../Images/8k_venus.jpg");
	CreateTexture(texTab[3], GL_RGB, "../Images/8k_earth.jpg");
	CreateTexture(texTab[4], GL_RGB, "../Images/8k_mars.jpg");
	CreateTexture(texTab[5], GL_RGB, "../Images/8k_jupiter.jpg");
	CreateTexture(texTab[6], GL_RGB, "../Images/8k_saturn.jpg");
	CreateTexture(texTab[7], GL_RGB, "../Images/2k_uranus.jpg");
	CreateTexture(texTab[8], GL_RGB, "../Images/2k_neptune.jpg");
	CreateTexture(texTab[9], GL_RGB, "../Images/2k_pluto.jpg");

	CreateTexture(texTab[10], GL_RGB, "../Images/Satellites/orbit.jpg");

	CreateCubemap(texTab[11]);

	CreateTexture(texTab[12], GL_RGB, "../Images/Satellites/8k_luna.jpg");

	CreateTexture(texTab[13], GL_RGB, "../Images/Satellites/callisto.jpg");
	CreateTexture(texTab[14], GL_RGB, "../Images/Satellites/europa.jpg");
	CreateTexture(texTab[15], GL_RGB, "../Images/Satellites/ganymede.jpg");
	CreateTexture(texTab[16], GL_RGB, "../Images/Satellites/io.jpg");
	CreateTexture(texTab[17], GL_RGB, "../Images/Satellites/titan.png");
	CreateTexture(texTab[18], GL_RGB, "../Images/Satellites/triton.jpg");



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

	// Milky Way background
	Skybox * sb = new Skybox();

	// Sun
	Sphere * sun = new Sphere(sun_radius / 2.0f);

	// Planets, plutoid and orbits
	Sphere * planets[9];
	Orbit * orbits[10];
	for (int i = 0; i < 9; ++i)
	{
		planets[i] = new Sphere(radii[i]);
		orbits[i] = new Orbit(distances[i]);
	}

	// Moon of Earth
	float luna_radius = 0.273f;
	Sphere * luna = new Sphere(luna_radius);
	float earth_luna_distance = earth_radius + 0.384f * factor;
	float luna_distance = earth_distance + earth_luna_distance;
	orbits[9] = new Orbit(earth_luna_distance);

	// All moons > pluto size
	std::vector<float> moonRadii;
	float callisto_radius = 0.378f;
	moonRadii.push_back(callisto_radius);
	float europa_radius = 0.245f;
	moonRadii.push_back(europa_radius);
	float ganymede_radius = 0.413f;
	moonRadii.push_back(ganymede_radius);
	float io_radius = 0.286f;
	moonRadii.push_back(io_radius);
	float titan_radius = 0.404f;
	moonRadii.push_back(titan_radius);
	float triton_radius = 0.212f;
	moonRadii.push_back(triton_radius);

	// All distance moons to their corresponding planets
	std::vector<float> moonDistances;
	float jupiter_callisto_distance = jupiter_radius + 1.883000f * factor;
	moonDistances.push_back(jupiter_callisto_distance);
	float jupiter_europa_distance = jupiter_radius + 0.671000f * factor;
	moonDistances.push_back(jupiter_europa_distance);
	float jupiter_ganymede_distance = jupiter_radius + 1.070000f * factor;
	moonDistances.push_back(jupiter_ganymede_distance);
	float jupiter_io_distance = jupiter_radius + 0.422000f * factor;
	moonDistances.push_back(jupiter_io_distance);
	float saturn_titan_distance = saturn_radius + 1.222000f * factor;
	moonDistances.push_back(saturn_titan_distance);
	float neptune_triton_distance = neptune_radius + 0.354800f * factor;
	moonDistances.push_back(neptune_triton_distance);

	Sphere * moons[6];
	Orbit * moonOrbits[6];
	for (int i = 0; i < 6; ++i)
	{
		moons[i] = new Sphere(moonRadii[i]);
		moonOrbits[i] = new Orbit(moonDistances[i]);
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
		// Set far plane value to a sufficiently high one to be able to visualize all meshes
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 1000.0f);

		// Calculate the VIEW matrix (simulate a camera circling around the scene)
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();

		// Calculate the MODEL matrix for the sun (with translations applied to center it) 
		glm::mat4 modelSun = glm::mat4(1.0f);
		modelSun = glm::translate(modelSun, sunPos);
		modelSun = glm::rotate(modelSun, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		// Activate the shader to initialize projection, view then model mat4 inside it
		shaderSphere.use();
		shaderSphere.setMat4("projection", projection);
		shaderSphere.setMat4("view", view);
		shaderSphere.setMat4("model", modelSun);

		// Activate the texture unit before binding texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texTab[0]);

		shaderSphere.setInt("texSampler", 0);	// SEEM UNEFFECTIVE

		sun->Draw();



		// Render the 10 planets orbiting around the sun and their orbits
		for (unsigned int i = 0; i < 9; i++)
		{
			// Calculate the MODEL matrix for each cube (with translations applied to see them all) 
			glm::mat4 modelPlanet = glm::mat4(1.0f);

			float anglePl = (float)(glfwGetTime() * (i+1));
			float anglePlInRad = glm::radians(anglePl);

			// ORBITAL MOTION : rotation around the sun
			modelPlanet = glm::translate(modelPlanet, glm::vec3(distances[i] * sin(anglePlInRad), 0.0f, distances[i] * cos(anglePlInRad)));
			// Rotation (permanent since the beginning) on itself to have texture seen horizontaly
			modelPlanet = glm::rotate(modelPlanet, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			// PROGRADE MOTION (not retrograde) : rotation on itself
			modelPlanet = glm::rotate(modelPlanet, anglePl, glm::vec3(0.0f, 0.0f, 1.0f));

			shaderSphere.use();
			shaderSphere.setMat4("model", modelPlanet);

			// Activate the texture unit before binding texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texTab[i + 1]);

			shaderSphere.setInt("texSampler", 0); // SEEM UNEFFECTIVE

			planets[i]->Draw();

			// Drawing Luna and its orbit
			if (i == 2)
			{
				glm::mat4 modelLuna = glm::mat4(1.0f);

				float angleLuna = (float)(glfwGetTime() * (i + 10));
				float angleLunaInRad = glm::radians(angleLuna);

				modelLuna = glm::translate(modelLuna, glm::vec3(earth_distance * sin(anglePlInRad), 0.0f, earth_distance * cos(anglePlInRad)));
				modelLuna = glm::translate(modelLuna, glm::vec3(earth_luna_distance * sin(angleLunaInRad), 0.0f, earth_luna_distance * cos(angleLunaInRad)));
				modelLuna = glm::rotate(modelLuna, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelLuna = glm::rotate(modelLuna, angleLuna, glm::vec3(0.0f, 0.0f, 1.0f));

				shaderSphere.use();
				shaderSphere.setMat4("model", modelLuna);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texTab[12]);
				shaderSphere.setInt("texSampler", 0); // SEEM UNEFFECTIVE

				luna->Draw();

				// Drawing planet orbites
				glm::mat4 modelOrbit = glm::mat4(1.0f);
				modelOrbit = glm::translate(modelOrbit, glm::vec3(earth_distance * sin(anglePlInRad), 0.0f, earth_distance * cos(anglePlInRad)));
				shaderSphere.use();
				shaderSphere.setMat4("model", modelOrbit);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texTab[10]);
				shaderSphere.setInt("texSampler", 0); // SEEM UNEFFECTIVE
				orbits[9]->Draw();
			}

			// Drawing galilean moons and its respective orbites
			else if (i == 4)
			{
				for (unsigned int j = 0; j < 4; ++j)
				{
					glm::mat4 modelGalileanMoon = glm::mat4(1.0f);

					float angleMoon = (float)(glfwGetTime() * (i + 10));
					float angleMoonInRad = glm::radians(angleMoon);

					modelGalileanMoon = glm::translate(modelGalileanMoon, glm::vec3(jupiter_distance * sin(anglePlInRad), 0.0f, jupiter_distance * cos(anglePlInRad)));
					modelGalileanMoon = glm::translate(modelGalileanMoon, glm::vec3(moonDistances[j] * sin(angleMoonInRad), 0.0f, moonDistances[j] * cos(angleMoonInRad)));
					modelGalileanMoon = glm::rotate(modelGalileanMoon, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					modelGalileanMoon = glm::rotate(modelGalileanMoon, angleMoon, glm::vec3(0.0f, 0.0f, 1.0f));

					shaderSphere.use();
					shaderSphere.setMat4("model", modelGalileanMoon);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, texTab[13 + j]);
					shaderSphere.setInt("texSampler", 0); // SEEM UNEFFECTIVE

					moons[j]->Draw();

					// Drawing planet orbites
					glm::mat4 modelGalileanOrbit = glm::mat4(1.0f);
					modelGalileanOrbit = glm::translate(modelGalileanOrbit, glm::vec3(jupiter_distance * sin(anglePlInRad), 0.0f, jupiter_distance * cos(anglePlInRad)));
					shaderSphere.use();
					shaderSphere.setMat4("model", modelGalileanOrbit);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, texTab[10]);
					shaderSphere.setInt("texSampler", 0); // SEEM UNEFFECTIVE
					moonOrbits[j]->Draw();
				}
			}

			// Drawing Titan and its orbit
			if (i == 5)
			{
				glm::mat4 modelTitan = glm::mat4(1.0f);

				float angleTitan = (float)(glfwGetTime() * (i + 10));
				float angleTitanInRad = glm::radians(angleTitan);

				modelTitan = glm::translate(modelTitan, glm::vec3(saturn_distance * sin(anglePlInRad), 0.0f, saturn_distance * cos(anglePlInRad)));
				modelTitan = glm::translate(modelTitan, glm::vec3(saturn_titan_distance * sin(angleTitanInRad), 0.0f, saturn_titan_distance * cos(angleTitanInRad)));
				modelTitan = glm::rotate(modelTitan, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTitan = glm::rotate(modelTitan, angleTitan, glm::vec3(0.0f, 0.0f, 1.0f));

				shaderSphere.use();
				shaderSphere.setMat4("model", modelTitan);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texTab[17]);
				shaderSphere.setInt("texSampler", 0); // SEEM UNEFFECTIVE

				moons[4]->Draw();

				// Drawing planet orbites
				glm::mat4 modelOrbit = glm::mat4(1.0f);
				modelOrbit = glm::translate(modelOrbit, glm::vec3(saturn_distance * sin(anglePlInRad), 0.0f, saturn_distance * cos(anglePlInRad)));
				shaderSphere.use();
				shaderSphere.setMat4("model", modelOrbit);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texTab[10]);
				shaderSphere.setInt("texSampler", 0); // SEEM UNEFFECTIVE
				moonOrbits[4]->Draw();
			}

			// Drawing Triton and its orbit
			if (i == 7)
			{
				glm::mat4 modelTriton = glm::mat4(1.0f);

				float angleTriton = (float)(glfwGetTime() * (i + 10));
				float angleTritonInRad = glm::radians(angleTriton);

				modelTriton = glm::translate(modelTriton, glm::vec3(neptune_distance * sin(anglePlInRad), 0.0f, neptune_distance * cos(anglePlInRad)));
				modelTriton = glm::translate(modelTriton, glm::vec3(neptune_triton_distance * sin(angleTritonInRad), 0.0f, neptune_triton_distance * cos(angleTritonInRad)));
				modelTriton = glm::rotate(modelTriton, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTriton = glm::rotate(modelTriton, angleTriton, glm::vec3(0.0f, 0.0f, 1.0f));

				shaderSphere.use();
				shaderSphere.setMat4("model", modelTriton);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texTab[18]);
				shaderSphere.setInt("texSampler", 0); // SEEM UNEFFECTIVE

				moons[5]->Draw();

				// Drawing planet orbites
				glm::mat4 modelOrbit = glm::mat4(1.0f);
				modelOrbit = glm::translate(modelOrbit, glm::vec3(neptune_distance * sin(anglePlInRad), 0.0f, neptune_distance * cos(anglePlInRad)));
				shaderSphere.use();
				shaderSphere.setMat4("model", modelOrbit);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texTab[10]);
				shaderSphere.setInt("texSampler", 0); // SEEM UNEFFECTIVE
				moonOrbits[5]->Draw();
			}

			// Drawing planet orbites
			glm::mat4 modelOrbit = glm::mat4(1.0f);
			shaderSphere.use();
			shaderSphere.setMat4("model", modelOrbit);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texTab[10]);
			shaderSphere.setInt("texSampler", 0); // SEEM UNEFFECTIVE
			orbits[i]->Draw();
		}

		glDepthFunc(GL_LEQUAL);		// change depth function so depth test passes when values are equal to depth buffer's content
		glm::mat4 projectionSb = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewSb = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		shaderSkybox.use();
		shaderSkybox.setMat4("view", viewSb);
		shaderSkybox.setMat4("projection", projectionSb);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texTab[11]);
		shaderSkybox.setInt("texSampler", 0);
		sb->Draw();
		glDepthFunc(GL_LESS);			// set depth function back to default



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


