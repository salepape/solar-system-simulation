#include "Window.h"

#include <glfw3.h>
#include <iostream>

#include "Utils.h"



Window::Window(const uint32_t inWidth, const uint32_t inHeight, const std::string& inTitle) :
	title(inTitle)
{
	Resize(inWidth, inHeight);

	lastCursorPosition = glm::vec2(0.5f * width, 0.5f * height);

	GLFWWindow = InitGLFWWindow();
	if (GLFWWindow == nullptr)
	{
		std::cout << "ERROR::GLFW - Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return;
	}

	Utils::SetGLFWCallbackData(GLFWWindow, this);

	if (MakeContextCurrent() == -1)
	{
		return;
	}

	Callback_DetectWindowResize();

	SetCursorMode(GLFW_CURSOR_DISABLED);
}

GLFWwindow* const Window::InitGLFWWindow()
{
	// Initialise GLFW library
	glfwInit();

	// Set major and minor version of OpenGL, to prevent others who have not this OpenGL version GLFW to fail
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// Tell explicitly GLFW that we want to use OpenGL core profile 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

int32_t Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(GLFWWindow);

	if (glfwGetCurrentContext() == nullptr)
	{
		std::cout << "ERROR::GLFW - Failed to get current context: OpenGL functions will not work correctly!" << std::endl;
		return -1;
	}

	return 0;
}

void Window::SwapFrontAndBackBuffers()
{
	glfwSwapBuffers(GLFWWindow);
}

void Window::ProcessPendingEvents()
{
	glfwPollEvents();
}

void Window::ClearResources()
{
	glfwTerminate();
}

void Window::Callback_DetectWindowResize()
{
	glfwSetFramebufferSizeCallback(GLFWWindow, [](GLFWwindow* GLFWWindow, int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);

		// Access contextual data from within GLFWwindow callback
		Window* const window = Utils::GetGLFWCallbackData(GLFWWindow);
		if (window == nullptr)
		{
			std::cout << "ERROR::WINDOW - Failed to cast glfwGetWindowUserPointer()!" << std::endl;
			return;
		}

		window->Resize(width, height);
	});
}

void Window::Resize(const uint32_t newWidth, const uint32_t newHeight)
{
	width = newWidth;
	height = newHeight;
	aspectRatio = width * 1.0f / height;
}

void Window::SetCursorMode(const int modeValue)
{
	glfwSetInputMode(GLFWWindow, GLFW_CURSOR, modeValue);
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