#include "Window.h"

#include <glfw/glfw3.h>

#include <cassert>
#include <iostream>

#include "Utils/Helpers.h"



Window::Window(const uint32_t inWidth, const uint32_t inHeight, const std::string& inTitle) :
	width(inWidth),
	height(inHeight),
	title(inTitle),
	aspectRatio(width * 1.0f / height),
	lastCursorPosition(glm::vec2(0.5f * width, 0.5f * height))
{
	// Initialise GLFW library before any GLFW function calls
	const int isGlfwInitialised = glfwInit();
	if (isGlfwInitialised == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW - Failed to initialise GLFW3 library properly: closing the application..." << std::endl;
		assert(false);
	}

	lastCursorPosition = glm::vec2(0.5f * width, 0.5f * height);

	GLFWHelper::SetErrorHandlingGLFWCallback();

	InitGLFWWindowAndOpenGLContext();
	if (GLFWWindow == nullptr)
	{
		std::cout << "ERROR::GLFW - Failed to create GLFW Window and its associated OpenGL Context!" << std::endl;
		assert(false);
	}

	GLFWHelper::SetGLFWWindowPointerToUserData(GLFWWindow, this);

	MakeOpenGLContextCurrent();

	SetWindowResizeGLFWCallback();

	SetCursorMode(GLFW_CURSOR_DISABLED);
}

Window::~Window()
{
	ClearGLFWWindowAndOpenGLContext();
	GLFWWindow = nullptr;

	// Free any resource related to all GLFW Windows and library
	glfwTerminate();
}

void Window::InitGLFWWindowAndOpenGLContext()
{
	// Set OpenGL major.minor version to one supported by large range of GPUs (check 'dxdiag' then click 'Display' section
	// for DirectX 12 max feature level: if OpenGL version not supported, GPU or installed driver too old, and program will abort)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Set OpenGL profile to Core, to not get all deprecated functions that are not needed anymore on Windows OS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Warning: GLFW function includes the creation of an OpenGL Context!
	GLFWWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

void Window::ClearGLFWWindowAndOpenGLContext() const
{
	glfwDestroyWindow(GLFWWindow);
}

void Window::MakeOpenGLContextCurrent() const
{
	glfwMakeContextCurrent(GLFWWindow);

	const GLFWwindow* const windowWithCurrentOpenGLContext = glfwGetCurrentContext();
	if (windowWithCurrentOpenGLContext == nullptr)
	{
		std::cout << "ERROR::WINDOW - Failed to get current OpenGL Context: OpenGL functions will not work correctly!" << std::endl;
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

void Window::SetWindowResizeGLFWCallback() const
{
	glfwSetFramebufferSizeCallback(GLFWWindow, [](GLFWwindow* GLFWWindow, int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);

		Window* const window = GLFWHelper::GetGLFWWindowPointerToUserData(GLFWWindow);
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
