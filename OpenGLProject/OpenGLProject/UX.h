#include <glad/glad.h>
#include <glfw3.h>

#include "Camera.h"

#define GLFW_INCLUDE_NONE



// Constants for distance and speed scales
float ACCELERATION = 1.0;

// Initial dimensions for the graphics window
constexpr unsigned int SCR_WIDTH = 1000;
constexpr unsigned int SCR_HEIGHT = 1000;

// Variables for mouse callback function
bool firstMouseInput = true;
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;

// Variables for time related to frames
float currentFrame = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool paused = false;



// Camera object initialization
Camera camera(glm::vec3(0.0f, 50.0f, 200.0f));



///////////////////////// WINDOW /////////////////////////

GLFWwindow* initGLFWWindow(int windowWidth, int windowHeight, const char* windowTitle)
{
	// Initialization of a GLFW window
	glfwInit();

	// Set major and minor version of OpenGL to 3, to prevent others who have not this OpenGL version GLFW to fail
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell explicitly GLFW that we want to use core-profile 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Creation of a GLFW window of size "width x height", its title appearing in a top bar
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

	return window;
}





///////////////////////// USER INTERACTIVITY /////////////////////////

// Check whether a specific key is pressed / released 
void processInput(GLFWwindow* window)
{
	// Quit the simulation
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Increase camera speed
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && camera.MovementSpeed <= 2.0f * SPEED)
		camera.MovementSpeed *= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE && camera.MovementSpeed >= SPEED)
		camera.MovementSpeed /= 2.0f;

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
		ACCELERATION *= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		ACCELERATION /= 2.0f;
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

	camera.ProcessMouseMovement(static_cast<float>(xoffset), static_cast<float>(yoffset), firstMouseInput);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}