#include "Window.h"

#include <glfw3.h>
#include <iostream>



Window::Window(const uint32_t inWidth, const uint32_t inHeight, const std::string& inTitle) :
	title(inTitle)
{
	UpdateDimensions(inWidth, inHeight);

	lastCursorPosition = glm::vec2(0.5f * width, 0.5f * height);

	GLFWWindow = initGLFWWindow();
	if (GLFWWindow == nullptr)
	{
		std::cout << "ERROR::GLFW - Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	// Register contextual data to access it from within GLFWwindow callbacks (their prototype cannot be modified at all) later
	glfwSetWindowUserPointer(GLFWWindow, static_cast<void*>(this));

	if (MakeContextCurrent() == -1)
	{
		return;
	}

	Callback_DetectWindowResize();

	SetCursorMode(GLFW_CURSOR_DISABLED);
}

GLFWwindow* const Window::initGLFWWindow()
{
	// Initialization of a GLFW window
	glfwInit();

	// Set major and minor version of OpenGL to 3, to prevent others who have not this OpenGL version GLFW to fail
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// Tell explicitly GLFW that we want to use core-profile 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creation of a GLFW window of size "width x height", its title appearing on a top bar
	return glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

int32_t Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(GLFWWindow);

	if (glfwGetCurrentContext() == nullptr)
	{
		std::cout << "ERROR::GLFW - Failed to get current context : OpenGL functions will not work correctly" << std::endl;
		return -1;
	}

	return 0;
}

void Window::Callback_DetectWindowResize()
{
	glfwSetFramebufferSizeCallback(GLFWWindow, [](GLFWwindow* GLFWWindow, int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);

		// Access contextual data from within GLFWwindow callback
		auto* const window = static_cast<Window*>(glfwGetWindowUserPointer(GLFWWindow));
		if (window == nullptr)
		{
			std::cout << "ERROR::WINDOW - Failed to cast glfwGetWindowUserPointer()!" << std::endl;
			return;
		}

		window->UpdateDimensions(width, height);
	});
}

void Window::SetCursorMode(const int modeValue)
{
	glfwSetInputMode(GLFWWindow, GLFW_CURSOR, modeValue);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(GLFWWindow);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::FreeUpResources()
{
	glfwTerminate();
}

void Window::UpdateDimensions(const uint32_t newWidth, const uint32_t newHeight)
{
	width = newWidth;
	height = newHeight;
	aspectRatio = width * 1.0f / height;
}

const glm::vec2 Window::ComputeCursorOffset(const double xPosition, const double yPosition)
{
	// Avoid little jump 
	if (isFirstMouseInput)
	{
		lastCursorPosition = glm::vec2(xPosition, yPosition);
		isFirstMouseInput = false;
	}

	// Reverse y-coordinates since they go from bottom to top  
	const glm::vec2& cursorOffset = glm::vec2(xPosition - lastCursorPosition.x, lastCursorPosition.y - yPosition);
	lastCursorPosition = glm::vec2(xPosition, yPosition);

	return cursorOffset;
}
