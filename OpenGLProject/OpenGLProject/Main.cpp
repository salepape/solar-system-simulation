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
float FACTOR2 = 1.0;

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

	"../Images/asteroid.tga",
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





// Check whether a specific key is pressed / released 
void processInput(GLFWwindow* window)
{
	// Quit the simulation
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Enable camera to move forward, backward, up, down, left and right (designed for AZERTY keyboards with corresponding QWERTY GLFW_KEYs)
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

	// Pause the simulation
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		paused = true;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		paused = false;
		glfwSetTime(lastFrame);
	}

	// Speed up / Slow down the simulation
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		FACTOR2 *= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		FACTOR2 /= 2.0f;
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
	Shader sphereShader("SphereShader.vs", "SphereShader.fs");
	Shader skyboxShader("SkyboxShader.vs", "SkyboxShader.fs");
	Shader asteroidShader("AsteroidShader.vs", "AsteroidShader.fs");

	// Load models
	Model saturnRings("../Models/SaturnRings.obj");
	Model asteroid("../Models/rock.obj");





	// Milky Way skybox
	Skybox * sb = new Skybox();

	// GENERATION TAO - Generate TOs storing its ID within texArray (within graphics memory)
	int spritesSize = sprites.size();
	GLuint texTab[22];
	CreateTexture(texTab);
	CreateSkybox(texTab[spritesSize]);

	// Texture index
	// Planets and moons (only those > pluto radius in length) radii divided by earth's radius value [in kms]
	// Distances between planets (resp. moons) and sun (resp. the planet around which they turn) divided by earth's distance value [in kms]
	// Or axial tilt : angle between planet / moon axis rotation and the normal of its orbital plane [in degrees]
	// Time the planet (resp. moon) takes to do 1 revolution around the sun (resp. its planet) [in earth days]
	// Time the planet takes to do a rotation on itself [in earth days]
	// Or "orbital tilt" : angle between planet / moon orbit and the ecliptic [in degrees]
	// Sphere mesh
	// Orbit mesh
	// Reference to the planet object to be able to make computation for its moon
	// Angle of planet rotation around the sun per frame [in radians]
	struct EntityInfo
	{
		GLuint textID;
		float radius;
		float dist;
		float obliquity;
		float orbPeriod;
		float rotPeriod;
		Sphere * sphere;
		float orbInclination;
		Orbit * orbit;
		EntityInfo * planet;
		float angleRot;
	};

	// Solar System data
	std::unordered_map<std::string, EntityInfo> data;
	using dataElmt = std::pair<std::string, EntityInfo>;

	data.insert(dataElmt("Sun", { texTab[0],  109.3f, 0.0f, 7.25f, 0.0f, 27.0f, nullptr, 0.0f, nullptr, nullptr, 0.0f })); 

	data.insert(dataElmt("Mercury", { texTab[1],  0.383f, data["Sun"].radius + 0.38f * FACTOR, 0.03f, 87.97f, 58.6f, nullptr, 7.01f, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Venus", { texTab[2],  0.95f, data["Mercury"].dist + 0.72f * FACTOR, 2.64f, 224.7f, -243.02f, nullptr, 3.39f, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Earth", { texTab[3],  1.0f,	data["Venus"].dist + 1.0f * FACTOR, 23.44f, 365.26f, 0.99f, nullptr, 0.0f, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Mars", { texTab[4],  0.532f, data["Earth"].dist + 1.52f * FACTOR, 25.19f, 686.97f, 1.03f, nullptr, 1.85f, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Jupiter", { texTab[5],  10.97f, data["Mars"].dist + 5.19f * FACTOR, 3.13f, 4332.59f, 0.41f, nullptr, 1.31f, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Saturn", { texTab[6],  9.14f, data["Jupiter"].dist + 9.53f * FACTOR, 26.73f, 10759.22f, 0.43f, nullptr, 2.49f, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Uranus", { texTab[7],  3.981f, data["Saturn"].dist + 19.20f * FACTOR, 82.23f, 30688.5, -0.72f, nullptr, 0.77f, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Neptune", { texTab[8],  3.865f, data["Uranus"].dist + 30.05f * FACTOR, 28.32f, 60182.0f, 0.67f, nullptr, 1.77f, nullptr, nullptr, 0.0f }));

	data.insert(dataElmt("Ceres", { texTab[9],  0.074f, data["Mars"].dist + 2.75f * FACTOR, 0.0f, 1683.15f, 9.07f, nullptr, 17.14f, nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Pluto", { texTab[10],  0.186f, data["Neptune"].dist + 39.24f * FACTOR, 57.47f, 90560.0f, -6.39f, nullptr, 10.62f, nullptr, nullptr, 0.0f }));

	data.insert(dataElmt("Luna", { texTab[11], 0.273f, data["Earth"].radius + 0.384f * FACTOR, 6.68f, 27.32f, 27.32f, nullptr, 5.145f, nullptr, &data["Earth"] }));
	data.insert(dataElmt("Callisto", { texTab[12], 0.378f, data["Jupiter"].radius + 1.883f * FACTOR, 0.0f, 16.69f, 16.69f, nullptr, 2.017f, nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Europa", { texTab[13], 0.245f, data["Jupiter"].radius + 0.671f * FACTOR, 0.1f, 3.55f, 3.55f, nullptr, 1.791f, nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Ganymede", { texTab[14], 0.413f, data["Jupiter"].radius + 1.07f * FACTOR, 0.16f, 7.15f, 7.15f, nullptr, 2.214f, nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Io", { texTab[15], 0.286f, data["Jupiter"].radius + 0.422f * FACTOR, 0.0f, 1.77f, 1.77f, nullptr, 2.213f, nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Titan", { texTab[16], 0.404f, data["Saturn"].radius + 1.222f * FACTOR, 0.0f, 15.95f, 15.95f, nullptr, 0.0f, nullptr, &data["Saturn"] }));
	data.insert(dataElmt("Triton", { texTab[17], 0.212f, data["Neptune"].radius + 0.354f * FACTOR, 0.0f, 5.88f, 5.88f, nullptr, 129.812f, nullptr, &data["Neptune"] }));

	for (auto it = data.begin(); it != data.end(); ++it)
	{
		if (it->first == "Sun")
			it->second.sphere = new Sphere(it->second.radius / 2.0f);
		else
		{
			it->second.sphere = new Sphere(it->second.radius);
			it->second.orbit = new Orbit(it->second.dist);
		}
	}




	struct Belt
	{
		int asteroidNb;
		int sizeFactor;
		float radius;
		float tubeRadius;
	};

	Belt asteroidBelt = { 5000, 10, data["Mars"].dist * 1.1f, 2.75f * FACTOR / 2.5f };
	Belt kuiperBelt = { 20000, 20, data["Neptune"].dist * 1.4f, 30.05f * FACTOR };
	static const int nbBelts = 2;
	Belt belts[nbBelts] = { asteroidBelt, kuiperBelt };

	// Generate large list of semi-random model transformation matrices, each representing an asteroid in a belt
	glm::mat4 * modelMatrices;
	int totalNbAsteroid = asteroidBelt.asteroidNb + kuiperBelt.asteroidNb;
	modelMatrices = new glm::mat4[totalNbAsteroid];
	srand((unsigned int)glfwGetTime());					// Initialize random seed	

	int k = 0;
	for (int b = 0; b < nbBelts; ++b)
	{
		for (int i = k + 0; i < k + belts[b].asteroidNb; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			// TRANSLATION : compute random position within the belt tore
			float angle = (float)i / (float)belts[b].asteroidNb * 360.0f;
			float displacement = (rand() % (int)(2 * belts[b].tubeRadius * 100)) / 100.0f - belts[b].tubeRadius;
			float x = sin(angle) * belts[b].radius + displacement;
			displacement = (rand() % (int)(2 * belts[b].tubeRadius * 100)) / 100.0f - belts[b].tubeRadius;
			float y = displacement * 0.4f;					// keep height of asteroid field smaller compared to width of x and z
			displacement = (rand() % (int)(2 * belts[b].tubeRadius * 100)) / 100.0f - belts[b].tubeRadius;
			float z = cos(angle) * belts[b].radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// SCALE : resize between 0.05 and "0.05 + 0.sizeFactor"
			float scale = (float)(rand() % belts[b].sizeFactor) / 100.0f + 0.05f;
			model = glm::scale(model, glm::vec3(scale));

			// ROTATION : add random rotation around a (semi)-randomly picked rotation axis vector
			float rotAngle = (float)(rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// Add current model matrix to the list
			modelMatrices[i] = model;
		}

		k = belts[b].asteroidNb;
	}

	// Configure instanced array
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (totalNbAsteroid) * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	// Set transformation matrices as an instance vertex attribute (with divisor 1)
	for (unsigned int i = 0; i < asteroid.meshes.size(); i++)
	{
		unsigned int VAO = asteroid.meshes[i].VAO;
		glBindVertexArray(VAO);

		// Set attribute pointers for matrix (4 times vec4 due to GLSL vertex shader limitation)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}





	// RENDER LOOP (check at the start of each loop iteration if GLFW has been instructed to close every frame)
	while (!glfwWindowShouldClose(window))
	{
		if (!paused)
		{
			// Time elapsed since GLFW initialisation (considered as a dimensionless chrono, but in seconds in reality)
			currentFrame = (float)glfwGetTime();

			// Compute delta time in order to reduce differences between computer processing powers
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
		}

		// Check if a specific key is pressed and react accordingly
		processInput(window);

		// Not needed since we have a skybox
		//colorationGLFWWindow();

		// Clear the depth buffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





		// Calculate the PROJECTION matrix (simulate a zoom - set far plane variable to a sufficiently high value)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 1000.0f);

		// Calculate the VIEW matrix (simulate a camera circling around the scene)
		glm::mat4 view = camera.GetViewMatrix();

		// Activate the shader to initialize projection, view then model mat4 inside it
		sphereShader.use();
		sphereShader.setMat4("projection", projection);
		sphereShader.setMat4("view", view);



		// Drawing sun then planets / moons, their orbits and their motion
		for (auto it = data.begin(); it != data.end(); it++)
		{
			float angleRot, angleRotItself;
			if (it->first != "Sun")
			{
				// Angle of rotation around the sun (resp. planet) for planets (resp. moons) per frame
				angleRot = 2.0f * glm::pi<float>() * currentFrame / it->second.orbPeriod * FACTOR2;
				if (it->second.planet == nullptr)
					it->second.angleRot = angleRot;

				// Angle of rotation of the celestial body around itself per frame
				angleRotItself = 2.0f * glm::pi<float>() * currentFrame * it->second.rotPeriod * FACTOR2;
			}
			else
			{
				angleRot = 0.0f;
				angleRotItself = 0.0f;
			}



			// Calculate the MODEL matrix for the sun(simulate movements that affects the current celestial body)
			glm::mat4 modelSphere = glm::mat4(1.0f);

			// Orbital tilt (around axis colinear to orbit direction) + Circular translation along the orbit (equidistance to axis normal to orbital plane)
			modelSphere = glm::translate(modelSphere, glm::vec3(it->second.dist * cos(glm::radians(it->second.orbInclination)) * sin(angleRot), it->second.dist * sin(glm::radians(it->second.orbInclination)) * sin(angleRot), it->second.dist * cos(angleRot)));																																																																	
			
			// Circular translation around corresponding planet (for moons only)
			if (it->second.planet != nullptr)
				modelSphere = glm::translate(modelSphere, glm::vec3(it->second.planet->dist * cos(glm::radians(it->second.planet->orbInclination)) * sin(it->second.planet->angleRot), it->second.planet->dist * sin(glm::radians(it->second.planet->orbInclination)) * sin(it->second.planet->angleRot), it->second.planet->dist * cos(it->second.planet->angleRot)));
			
			// Axis tilt (around axis colinear to orbit direction)
			modelSphere = glm::rotate(modelSphere, glm::radians(it->second.obliquity), glm::vec3(1.0f, 0.0f, 0.0f));
			// Rotation on itself (around axis normal to orbital plane)
			modelSphere = glm::rotate(modelSphere, angleRotItself, glm::vec3(0.0f, 1.0f, 0.0f));				
			// Rotation on itself to have spheres poles vertical 
			modelSphere = glm::rotate(modelSphere, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
																						


			sphereShader.use();
			sphereShader.setMat4("model", modelSphere);

			// Activate the texture unit before binding texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, it->second.textID);

			sphereShader.setInt("texSampler", 0);					// SEEM UNEFFECTIVE

			it->second.sphere->Draw();



			if (it->first != "Sun")
			{
				// Drawing planet orbites
				glm::mat4 modelOrbit = glm::mat4(1.0f);
				// Condition applies for moons only
				if (it->second.planet != nullptr)
					modelOrbit = glm::translate(modelOrbit, glm::vec3(it->second.planet->dist * cos(glm::radians(it->second.planet->orbInclination)) * sin(it->second.planet->angleRot), it->second.planet->dist * sin(glm::radians(it->second.planet->orbInclination)) * sin(it->second.planet->angleRot), it->second.planet->dist * cos(it->second.planet->angleRot)));
				modelOrbit = glm::rotate(modelOrbit, glm::radians(it->second.orbInclination), glm::vec3(0.0f, 0.0f, 1.0f));
				sphereShader.use();
				sphereShader.setMat4("model", modelOrbit);
				glActiveTexture(GL_TEXTURE0);
				if (it->second.planet != nullptr || it->first == "Pluto" || it->first == "Ceres")
					glBindTexture(GL_TEXTURE_2D, texTab[1]);
				else
					glBindTexture(GL_TEXTURE_2D, texTab[0]);
				sphereShader.setInt("texSampler", 0);
				it->second.orbit->Draw();
			}
		}



		// Drawing skybox
		glDepthFunc(GL_LEQUAL);				// Change depth function so that depth test passes when values are equal to depth buffer's content
		glm::mat4 projectionSb = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewSb = glm::mat4(glm::mat3(camera.GetViewMatrix())); 
		skyboxShader.use();
		skyboxShader.setMat4("view", viewSb);
		skyboxShader.setMat4("projection", projectionSb);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texTab[spritesSize]);
		skyboxShader.setInt("texSampler", 0);
		sb->Draw();
		glDepthFunc(GL_LESS);				// Set depth function back to default



		// Drawing Saturn rings
		glm::mat4 modelSaturnRings = glm::mat4(1.0f);
		auto saturnData = data["Saturn"];
		float angleRotSun = saturnData.angleRot;
		float angleRotItself = 2.0f * glm::pi<float>() * currentFrame * saturnData.rotPeriod * FACTOR2;
		modelSaturnRings = glm::translate(modelSaturnRings, glm::vec3(saturnData.dist * cos(glm::radians(saturnData.orbInclination)) * sin(angleRotSun), saturnData.dist * sin(glm::radians(saturnData.orbInclination)) * sin(angleRotSun), saturnData.dist * cos(angleRotSun)));																																																																	
		modelSaturnRings = glm::rotate(modelSaturnRings, glm::radians(saturnData.obliquity), glm::vec3(1.0f, 0.0f, 0.0f));
		modelSaturnRings = glm::rotate(modelSaturnRings, angleRotItself, glm::vec3(0.0f, 1.0f, 0.0f));			
		modelSaturnRings = glm::scale(modelSaturnRings, glm::vec3(0.05f, 0.05f, 0.05f));
		sphereShader.use();
		sphereShader.setMat4("model", modelSaturnRings);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texTab[spritesSize - 1]);
		sphereShader.setInt("texSampler", 0);
		saturnRings.Draw(sphereShader);





		// Drawing the 2 main belts composed of asteroids
		asteroidShader.use();
		asteroidShader.setMat4("projection", projection);
		asteroidShader.setMat4("view", view);
		asteroidShader.setInt("texSampler", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, asteroid.textures_loaded[0].id);		
		for (unsigned int i = 0; i < asteroid.meshes.size(); i++)
		{
			glBindVertexArray(asteroid.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, asteroid.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, totalNbAsteroid);
			glBindVertexArray(0);
		}





		// Swap font and back buffers (we sent to the screen the updated buffer)
		glfwSwapBuffers(window);

		// Check if any events are triggered, updates the window states and call the corresponding functions
		glfwPollEvents();
	}

	// Clean up properly all previous allocated GLFW resources
	glfwTerminate();

	return 0;
}


