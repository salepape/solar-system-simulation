#include "Window.h"

#include <glfw3.h>

#include "Application.h"
#include "Camera.h"
#include "InputHandler.h"



Window::Window(const unsigned int inWidth, const unsigned int inHeight, const std::string& inTitle) :
	width(inWidth), height(inHeight), title(inTitle)
{
	lastCursorPosition = { 0.5f * width, 0.5f * height };

	GLFWWindow = initGLFWWindow();
	if (GLFWWindow == nullptr)
	{
		std::cout << "ERROR::GLFW - Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	// Provide a Window pointer within the body of GLFW callbacks since we cannot add parameters to these function pointers/has to be non-capturing lambdas
	glfwSetWindowUserPointer(GLFWWindow, static_cast<void*>(this));

	if (MakeContextCurrent() == -1)
	{
		return;
	}

	SetFramebufferResizeCallback();
	SetCursorPositionCallback();
	SetScrollCallback();

	SetInputMode();
}

GLFWwindow* const Window::initGLFWWindow()
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
	return glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
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
	glfwSetCursorPosCallback(GLFWWindow, [](GLFWwindow* window, const double xPosition, const double yPosition)
	{
		auto* const self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (self == nullptr)
		{
			std::cout << "ERROR::WINDOW - Failed to reinterpret cast glfwGetWindowUserPointer()" << std::endl;
			return;
		}

		// Avoid little jump
		if (self->firstMouseInput)
		{
			self->lastCursorPosition.x = xPosition;
			self->lastCursorPosition.y = yPosition;
			self->firstMouseInput = false;
		}

		const float xOffset = static_cast<float>(xPosition - self->lastCursorPosition.x);

		// Reverse y-coordinates since they go from bottom to top
		const float yOffset = static_cast<float>(self->lastCursorPosition.y - yPosition);

		self->lastCursorPosition.x = xPosition;
		self->lastCursorPosition.y = yPosition;

		if (auto* const selfCamera = self->camera)
		{
			selfCamera->ProcessMouseMovement(xOffset, yOffset);
		}
	});
}

void Window::SetScrollCallback()
{
	glfwSetScrollCallback(GLFWWindow, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		auto* const self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (self == nullptr)
		{
			std::cout << "ERROR::WINDOW - Failed to reinterpret cast glfwGetWindowUserPointer()" << std::endl;
			return;
		}

		if (auto* const selfCamera = self->camera)
		{
			selfCamera->ProcessMouseScroll(static_cast<float>(yOffset));
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
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		Application::GetInstance().Close();
	}

	// Modify camera speed
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_X))
	{
		camera.IncreaseSpeed(2.0f);
	}
	if (InputHandler::GetInstance().IsKeyReleased(GLFW_KEY_X))
	{
		camera.DecreaseSpeed(2.0f);
	}

	// Enable camera to move forward, backward, up, down, left and right (designed for AZERTY keyboards with corresponding QWERTY GLFW_KEYs)
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_W))
	{
		camera.ProcessKeyboard(FORWARD, Application::GetInstance().deltaTime);
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_S))
	{
		camera.ProcessKeyboard(BACKWARD, Application::GetInstance().deltaTime);
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_A))
	{
		camera.ProcessKeyboard(LEFT, Application::GetInstance().deltaTime);
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_D))
	{
		camera.ProcessKeyboard(RIGHT, Application::GetInstance().deltaTime);
	}
	if (InputHandler::GetInstance().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		camera.ProcessKeyboard(UP, Application::GetInstance().deltaTime);
	}
	if (InputHandler::GetInstance().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		camera.ProcessKeyboard(DOWN, Application::GetInstance().deltaTime);
	}

	// Pause/Unpause the simulation
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_SPACE))
	{
		Application::GetInstance().Pause(true);
	}
	if (InputHandler::GetInstance().IsKeyReleased(GLFW_KEY_SPACE))
	{
		Application::GetInstance().Pause(false);
	}

	// Speed up/Slow down the simulation
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_UP))
	{
		Application::GetInstance().ChangeSpeed(2.0f);
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_DOWN))
	{
		Application::GetInstance().ChangeSpeed(0.5f);
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

float Window::GetAspectRatio() const
{
	int width;
	int height;
	glfwGetWindowSize(GLFWWindow, &width, &height);

	return static_cast<float>(width) * 1.0f / height;
}
