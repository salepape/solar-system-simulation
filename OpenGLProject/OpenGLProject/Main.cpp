#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

#include "Shader.h"
#include "Camera.h"
#include "Sphere.h"
#include "Skybox.h"
#include "Orbit.h"
#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define FACTOR 10.0f

// Initial dimensions for the graphics window
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// Camera object initialization
Camera camera(glm::vec3(0.0f, 50.0f, 200.0f));

// Variables for mouse callback function
bool firstMouseInput = true;
double lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;

// Time related to frames
float currentFrame = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool paused = false;


std::vector<std::string> sprites
{
	"../Images/8k_sun.tga",

	"../Images/8k_mercury.tga",
	"../Images/8k_venus.tga",
	"../Images/8k_earth.tga",
	"../Images/8k_mars.tga",
	"../Images/8k_jupiter.tga",
	"../Images/8k_saturn.tga",
	"../Images/2k_uranus.tga",
	"../Images/2k_neptune.tga",

	"../Images/4k_ceres_fictional.tga",
	"../Images/2k_pluto.tga",

	"../Images/Satellites/8k_luna.tga",
	"../Images/Satellites/callisto.tga",
	"../Images/Satellites/europa.tga",
	"../Images/Satellites/ganymede.tga",
	"../Images/Satellites/io.tga",
	"../Images/Satellites/titan.tga",
	"../Images/Satellites/triton.tga",

	"../Images/saturn_ring.tga"
};

std::vector<std::string> faces
{
	"../Images/skybox/stars.tga",				// right			
	"../Images/skybox/stars.tga",				// -> left
	"../Images/skybox/stars.tga",				// top 
	"../Images/skybox/stars.tga",				// bottom
	"../Images/skybox/stars.tga",				// back
	"../Images/skybox/stars.tga"				// -> front
};

//std::vector<std::string> faces
//{
//	"../Images/skybox/milky_way_right.tga",				// right
//	"../Images/skybox/milky_way_right.tga",				// -> left
//	"../Images/skybox/stars.tga",						// top 
//	"../Images/skybox/stars.tga",						// bottom
//	"../Images/skybox/milky_way_left.tga",				// back
//	"../Images/skybox/milky_way_left.tga"				// -> front
//};





// Check whether a specific key is pressed / released 
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
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		paused = true;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		paused = false;
		glfwSetTime(lastFrame);
	}
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
	// Reverse y-coordinates since they go from bottom to top
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

	// Set major and minor version of OpenGL to 3, to prevent others who have not this OpenGL version GLFW to fail
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell explicitly GLFW that we want to use core-profile 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creation of a GLFW window of size "width x height", its title appearing in a top bar
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

	return window;
}

void colorationGLFWWindow()
{
	// Set "RGB + alpha" coefficients for background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Clear up window content
	glClear(GL_COLOR_BUFFER_BIT);
}

void CreateTexture(GLuint texTab[])
{
	glGenTextures(sprites.size(), texTab);

	for (GLuint i = 0; i < sprites.size(); i++)
	{
		// BINDING T2O - Bind TO to the OpenGL TO type we want (here a 2D texture called GL_TEXTURE_2D, hence T2O variable name)
		glBindTexture(GL_TEXTURE_2D, texTab[i]);

		// Set the texture wrapping option (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set the texture filtering option (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char *data;

		// Tell stb_image.h to flip loaded texture's on the y-axis (to see textures applied normal to outward faces directions)
		stbi_set_flip_vertically_on_load(true);

		// Load image (without alpha channel)
		data = stbi_load(sprites[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			// Generate texture image on the currently bound TO at the active texture unit
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			// Generate all mipmap levels for the previous currently bound TO
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load entity sprite at index " << i << std::endl;
			stbi_image_free(data);
		}
	}
}

void CreateSkybox(GLuint& texSkyboxID)
{
	glGenTextures(1, &texSkyboxID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texSkyboxID);

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

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load skybox sprite at index " << i << std::endl;
			stbi_image_free(data);
		}
	}
}










int main()
{
	GLFWwindow* window = initGLFWWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Solar System Simulation");

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










	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_DEPTH_CLAMP);

	// Build and compile shader programs
	Shader shaderSphere("SphereShader.vs", "SphereShader.fs");
	Shader shaderSkybox("CubeShader.vs", "CubeShader.fs");

	// Load model
	Model saturnRings("../Models/SaturnRings.obj");

	// Milky Way skybox
	Skybox * sb = new Skybox();

	// GENERATION TAO - Generate TOs storing its ID within texArray (within graphics memory)
	int spritesSize = sprites.size();
	GLuint texTab[21];
	CreateTexture(texTab);
	CreateSkybox(texTab[spritesSize]);

	// Texture index
	// Planets and moons (only those > pluto radius in length) radii relative to earth's radius (= scaled so that earth radius = 1) (dimensionless)
	// Distances between planets (resp. moons) and sun (resp. the planet around which they turn) relative to earth's distance (dimensionless)
	// Or axial tilt : angle between planet / moon axis rotation and the normal of its orbital plane (in degrees)
	// Time the planet (resp. moon) takes to do 1 revolution around the sun (resp. its planet) (in earth days)
	// Time the planet takes to do a rotation on itself (in earth days)
	// Inclination of the orbit (in degrees)
	// Sphere mesh
	// Orbit mesh
	// Reference to the planet object to be able to make computation for its moon
	// Angle of rotation around the sun (in radians)
	struct EntityInfo
	{
		GLuint textID;
		float radius;
		float dist;
		float obliquity;
		float orbPeriod;
		float rotPeriod;
		// float orbInclination;
		Sphere * sphere;
		Orbit * orbit;
		EntityInfo * planet;
		float anglePlanet;
	};

	// Solar System data
	std::unordered_map<std::string, EntityInfo> data;
	using dataElmt = std::pair<std::string, EntityInfo>;

	float factor = 10.0f;

	data.insert(dataElmt("Sun",		{ texTab[0],  109.3f, 0.0f, 7.25f, 0.0f, 30.0f, nullptr, nullptr, nullptr, 0.0f }));

	data.insert(dataElmt("Mercury", { texTab[1],  0.383f, data["Sun"].radius + 0.38f * FACTOR, 0.03f, 87.97f, 58.6f, nullptr, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Venus",	{ texTab[2],  0.95f, data["Mercury"].dist + 0.72f * FACTOR, 2.64f, 224.7f, -243.02f, nullptr, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Earth",	{ texTab[3],  1.0f,	data["Venus"].dist + 1.0f * FACTOR, 23.44f, 365.26f, 0.99f, nullptr, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Mars",	{ texTab[4],  0.532f, data["Earth"].dist + 1.52f * FACTOR, 25.19f, 686.97f, 1.03f, nullptr, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Jupiter",	{ texTab[5],  10.97f, data["Mars"].dist + 5.19f * FACTOR, 3.13f, 4332.59f, 0.41f, nullptr, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Saturn",	{ texTab[6],  9.14f, data["Jupiter"].dist + 9.53f * FACTOR, 26.73f, 10759.22f, 0.43f, nullptr, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Uranus",	{ texTab[7],  3.981f, data["Saturn"].dist + 19.20f * FACTOR, 82.23f, 30688.5, -0.72f, nullptr, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Neptune", { texTab[8],  3.865f, data["Uranus"].dist + 30.05f * FACTOR, 28.32f, 60182.0f, 0.67f, nullptr, nullptr, nullptr, 0.0f }));

	data.insert(dataElmt("Ceres",	{ texTab[9],  0.074f, data["Mars"].dist + 2.75f * FACTOR, 0.0f, 1683.15f, 9.07f, nullptr, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Pluto",	{ texTab[10],  0.186f, data["Neptune"].dist + 39.24f * FACTOR, 57.47f, 90560.0f, -6.39f, nullptr, nullptr, nullptr, 0.0f }));

	data.insert(dataElmt("Luna",	{ texTab[11], 0.273f, data["Earth"].radius + 0.384f * FACTOR, 6.68f, 27.32f, 27.32f, nullptr, nullptr, &data["Earth"] }));
	data.insert(dataElmt("Callisto",{ texTab[12], 0.378f, data["Jupiter"].radius + 1.883f * FACTOR, 0.0f, 16.69f, 16.69f, nullptr, nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Europa",	{ texTab[13], 0.245f, data["Jupiter"].radius + 0.671f * FACTOR, 0.1f, 3.55f, 3.55f, nullptr, nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Ganymede",{ texTab[14], 0.413f, data["Jupiter"].radius + 1.07f * FACTOR, 0.16f, 7.15f, 7.15f, nullptr, nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Io",		{ texTab[15], 0.286f, data["Jupiter"].radius + 0.422f * FACTOR, 0.0f, 1.77f, 1.77f, nullptr, nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Titan",	{ texTab[16], 0.404f, data["Saturn"].radius + 1.222f * FACTOR, 0.0f, 15.95f, 15.95f, nullptr, nullptr, &data["Saturn"] }));
	data.insert(dataElmt("Triton",	{ texTab[17], 0.212f, data["Neptune"].radius + 0.354f * FACTOR, 0.0f, 5.88f, 5.88f, nullptr, nullptr, &data["Neptune"] }));

	for (auto it = data.begin(); it != data.end(); ++it)
	{
		if (it->first == "Sun")
			it->second.sphere = new Sphere(it->second.radius / 2.0f);
		else if (it->first == "Ceres" || it->first == "Pluto")
			it->second.sphere = new Sphere(it->second.radius);
		else
		{
			it->second.sphere = new Sphere(it->second.radius);
			it->second.orbit = new Orbit(it->second.dist);
		}
	}










	// RENDER LOOP (check at the start of each loop iteration if GLFW has been instructed to close every frame)
	while (!glfwWindowShouldClose(window))
	{
		// Time elapsed since GLFW initialisation (in seconds)
		if (!paused)
		{
			currentFrame = (float)glfwGetTime();

			// Compute delta time in order to reduce differences between computer processing powers
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
		}


		// Check if a specific key is pressed and react accordingly
		processInput(window);

		colorationGLFWWindow();





		// Clear the depth buffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate the PROJECTION matrix (simulate a zoom - set far plane variable to a sufficiently high value)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 1000.0f);

		// Calculate the VIEW matrix (simulate a camera circling around the scene)
		glm::mat4 view = camera.GetViewMatrix();

		// Calculate the MODEL matrix for the sun (simulate movements that affects the sun)
		glm::mat4 modelSun = glm::mat4(1.0f);
		// Rotation on itself to have texture seen horizontaly + obliquity angle
		modelSun = glm::rotate(modelSun, glm::radians(90.0f + data["Sun"].obliquity), glm::vec3(1.0f, 0.0f, 0.0f));
		// Rotation on itself
		modelSun = glm::rotate(modelSun, currentFrame / data["Sun"].rotPeriod, glm::vec3(0.0f, 0.0f, 1.0f));

		// Activate the shader to initialize projection, view then model mat4 inside it
		shaderSphere.use();
		shaderSphere.setMat4("projection", projection);
		shaderSphere.setMat4("view", view);
		shaderSphere.setMat4("model", modelSun);

		// Activate the texture unit before binding texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, data["Sun"].textID);

		shaderSphere.setInt("texSampler", 0);	// SEEM UNEFFECTIVE

		data["Sun"].sphere->Draw();





		// Drawing planets / moons, their orbits and their motion
		for (auto it = data.begin(); it != data.end(); it++)
		{
			if (it->first.compare("Sun") == 0)
				continue;
			else
			{
				// Only the model matrix differs from the sun
				glm::mat4 modelSphere = glm::mat4(1.0f);

				float angleRotSun = currentFrame / it->second.orbPeriod * it->second.dist / 10.0f;		
				if (it->second.planet == nullptr)
					it->second.anglePlanet = angleRotSun;
				if(it->second.planet != nullptr)
					angleRotSun += 10.0f;
				float angleRotItself = 2.0f * glm::pi<float>() * currentFrame / it->second.rotPeriod / 10.0f;

				// Circular translation around corresponding planet (for moons only)
				if (it->second.planet != nullptr)
					modelSphere = glm::translate(modelSphere, glm::vec3(it->second.planet->dist  * sin(it->second.planet->anglePlanet), 0.0f, it->second.planet->dist * cos(it->second.planet->anglePlanet)));
				// Circular translation around the sun
				modelSphere = glm::translate(modelSphere, glm::vec3(it->second.dist * sin(angleRotSun), 0.0f, it->second.dist * cos(angleRotSun)));
				// Rotation on itself to have texture seen horizontaly + obliquity angle
				modelSphere = glm::rotate(modelSphere, glm::radians(90.0f + it->second.obliquity), glm::vec3(1.0f, 0.0f, 0.0f));
				// Rotation on itself
				modelSphere = glm::rotate(modelSphere, angleRotItself, glm::vec3(0.0f, 0.0f, 1.0f));

				shaderSphere.use();
				shaderSphere.setMat4("model", modelSphere);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, it->second.textID);
				shaderSphere.setInt("texSampler", 0); 
				it->second.sphere->Draw();

				if (it->second.orbit != nullptr)
				{
					// Drawing planet orbites
					glm::mat4 modelOrbit = glm::mat4(1.0f);
					if (it->second.planet != nullptr)
						modelOrbit = glm::translate(modelOrbit, glm::vec3(it->second.planet->dist * sin(it->second.planet->anglePlanet), 0.0f, it->second.planet->dist * cos(it->second.planet->anglePlanet)));
					shaderSphere.use();
					shaderSphere.setMat4("model", modelOrbit);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, texTab[spritesSize - 1]);
					shaderSphere.setInt("texSampler", 0);
					it->second.orbit->Draw();
				}
			}
		}

		// Drawing skybox
		glDepthFunc(GL_LEQUAL);				// Change depth function so that depth test passes when values are equal to depth buffer's content
		glm::mat4 projectionSb = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewSb = glm::mat4(glm::mat3(camera.GetViewMatrix())); 
		shaderSkybox.use();
		shaderSkybox.setMat4("view", viewSb);
		shaderSkybox.setMat4("projection", projectionSb);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texTab[spritesSize]);
		shaderSkybox.setInt("texSampler", 0);
		sb->Draw();
		glDepthFunc(GL_LESS);				// Set depth function back to default





		// render the loaded model
		glm::mat4 modelSaturnRings = glm::mat4(1.0f);
		auto saturnData = data["Saturn"];
		float angleRotSun = saturnData.anglePlanet;
		float angleRotItself = 2.0f * glm::pi<float>() * currentFrame / saturnData.rotPeriod / 10.0f;
		modelSaturnRings = glm::translate(modelSaturnRings, glm::vec3(saturnData.dist * sin(angleRotSun), 0.0f, saturnData.dist * cos(angleRotSun)));
		modelSaturnRings = glm::rotate(modelSaturnRings, glm::radians(saturnData.obliquity), glm::vec3(1.0f, 0.0f, 0.0f));
		modelSaturnRings = glm::rotate(modelSaturnRings, angleRotItself, glm::vec3(0.0f, -1.0f, 0.0f));
		modelSaturnRings = glm::scale(modelSaturnRings, glm::vec3(0.05f, 0.05f, 0.05f));
		shaderSphere.use();
		shaderSphere.setMat4("model", modelSaturnRings);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texTab[spritesSize - 1]);
		shaderSphere.setInt("texSampler", 0);
		saturnRings.Draw(shaderSphere);





		//// Swap font and back buffers (we sent to the screen the updated buffer)
		glfwSwapBuffers(window);

		// Check if any events are triggered, updates the window states and call the corresponding functions
		glfwPollEvents();
	}

	// Clean up properly all previous allocated GLFW resources
	glfwTerminate();

	return 0;
}


