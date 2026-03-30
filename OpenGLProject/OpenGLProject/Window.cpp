#include "Window.h"

#include <cassert>
#include <glad/glad.h>
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
		assert(false);
	}

	GLFWUtils::SetGLFWCallbackData(GLFWWindow, this);

	MakeContextCurrent();

	Callback_DetectWindowResize();

	SetCursorMode(GLFW_CURSOR_DISABLED);
}

GLFWwindow* Window::InitGLFWWindow() const
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

void Window::MakeContextCurrent() const
{
	glfwMakeContextCurrent(GLFWWindow);

	if (glfwGetCurrentContext() == nullptr)
	{
		std::cout << "ERROR::GLFW - Failed to get current context: OpenGL functions will not work correctly!" << std::endl;
		assert(false);
	}
}

void Window::SwapFrontAndBackBuffers() const
{
	glfwSwapBuffers(GLFWWindow);
}

void Window::ProcessPendingEvents() const
{
	glfwPollEvents();
}

void Window::ClearResources() const
{
	glfwTerminate();
}

void Window::Callback_DetectWindowResize() const
{
	glfwSetFramebufferSizeCallback(GLFWWindow, [](GLFWwindow* GLFWWindow, int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);

		// Access contextual data from within GLFWwindow callback
		Window* const window = GLFWUtils::GetGLFWCallbackData(GLFWWindow);
		window->Resize(width, height);
	});
}

void Window::Resize(const uint32_t newWidth, const uint32_t newHeight)
{
	width = newWidth;
	height = newHeight;
	aspectRatio = width * 1.0f / height;
}

void Window::SetCursorMode(const int modeValue) const
{
	glfwSetInputMode(GLFWWindow, GLFW_CURSOR, modeValue);
}

glm::vec2 Window::ComputeCursorOffset(const double xPosition, const double yPosition)
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