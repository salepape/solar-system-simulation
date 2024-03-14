#include "Window.h"

#include <glfw3.h>

#include "Camera.h"



Window::Window(const unsigned int inWidth, const unsigned int inHeight, const char* inTitle) :
	width(inWidth), height(inHeight), title(inTitle)
{
	GLFWWindow = initGLFWWindow();
	if (GLFWWindow == nullptr)
	{
		std::cout << "ERROR::GLFW - Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	// Provide a Window pointer within the body of GLFW callbacks since we cannot add parameters to these function pointers/has to be non-capturing lambdas
	glfwSetWindowUserPointer(GLFWWindow, reinterpret_cast<void*>(this));

	lastXPos = width / 2.0f;
	lastYPos = height / 2.0f;

	if (MakeContextCurrent() == -1)
	{
		return;
	}

	SetFramebufferResizeCallback();
	SetCursorPositionCallback();
	SetScrollCallback();

	SetInputMode();
}

Window::~Window()
{

}

GLFWwindow* Window::initGLFWWindow()
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

	// Creation of a GLFW window of size "width x height", its title appearing on a top bar
	return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

int Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(GLFWWindow);

	if (glfwGetCurrentContext() == nullptr)
	{
		std::cout << "ERROR::GLFW - Failed to get current context : OpenGL functions will not work correctly" << std::endl;
		return -1;
	}

	return 0;
}

void Window::SetFramebufferResizeCallback()
{
	glfwSetFramebufferSizeCallback(GLFWWindow, [](GLFWwindow* window, int width, int height)
	{
		// Make sure the viewport matches the new window dimensions
		glViewport(0, 0, width, height);
	});
}

void Window::SetCursorPositionCallback()
{
	glfwSetCursorPosCallback(GLFWWindow, [](GLFWwindow* window, const double xPos, const double yPos)
	{
		auto* const self = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (self == nullptr)
		{
			std::cout << "ERROR::WINDOW - Failed to reinterpret cast glfwGetWindowUserPointer()" << std::endl;
			return;
		}

		// Avoid little jump
		if (self->firstMouseInput)
		{
			self->lastXPos = xPos;
			self->lastYPos = yPos;
			self->firstMouseInput = false;
		}

		const float xOffset = static_cast<float>(xPos - self->lastXPos);

		// Reverse y-coordinates since they go from bottom to top
		const float yOffset = static_cast<float>(self->lastYPos - yPos);

		self->lastXPos = xPos;
		self->lastYPos = yPos;

		if (self->camera)
		{
			self->camera->ProcessMouseMovement(xOffset, yOffset, self->firstMouseInput);
		}
	});
}

void Window::SetScrollCallback()
{
	glfwSetScrollCallback(GLFWWindow, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		auto* const self = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (self == nullptr)
		{
			std::cout << "ERROR::WINDOW - Failed to reinterpret cast glfwGetWindowUserPointer()" << std::endl;
			return;
		}

		if (self->camera)
		{
			self->camera->ProcessMouseScroll(static_cast<float>(yOffset));
		}
	});
}

void Window::SetInputMode()
{
	glfwSetInputMode(GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(GLFWWindow);
}

void Window::ProcessInput(Camera& camera)
{
	// Quit the simulation
	if (glfwGetKey(GLFWWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(GLFWWindow, true);
	}

	// Modify camera speed
	if (glfwGetKey(GLFWWindow, GLFW_KEY_X) == GLFW_PRESS)
	{
		camera.IncreaseSpeed(2.0f);
	}
	if (glfwGetKey(GLFWWindow, GLFW_KEY_X) == GLFW_RELEASE)
	{
		camera.DecreaseSpeed(2.0f);
	}

	// Enable camera to move forward, backward, up, down, left and right (designed for AZERTY keyboards with corresponding QWERTY GLFW_KEYs)
	if (glfwGetKey(GLFWWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(GLFWWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(GLFWWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(GLFWWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetMouseButton(GLFWWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetMouseButton(GLFWWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(DOWN, deltaTime);
	}

	// Pause/Unpause the simulation
	if (glfwGetKey(GLFWWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		simuPaused = true;
	}
	if (glfwGetKey(GLFWWindow, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		simuPaused = false;
		glfwSetTime(lastFrame);
	}

	// Speed up/Slow down the simulation
	if (glfwGetKey(GLFWWindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		simuSpeedFactor *= 2.0f;
	}
	if (glfwGetKey(GLFWWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		simuSpeedFactor /= 2.0f;
	}
}

bool Window::DoNotClose()
{
	return glfwWindowShouldClose(GLFWWindow) == 0;
}

void Window::UpdateFrames()
{
	if (simuPaused == false)
	{
		// Time elapsed since GLFW initialisation [considered as a dimensionless chrono, but in seconds in reality]
		currentFrame = static_cast<float>(glfwGetTime());

		// Compute delta time in order to reduce differences between computer processing powers
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::FreeUpResources()
{
	glfwTerminate();
}