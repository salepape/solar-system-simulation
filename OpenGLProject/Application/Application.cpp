#include "Application.h"

#include <glad/glad.h>	// No OpenGL functions called here, just setting up GLAD
#include <glfw/glfw3.h>

#include <algorithm>	// for std::min and std::max
#include <cassert>
#include <cstdint>
#include <iostream>
#include <utility>

#include "CoreEngine.h"
#include "Window.h"

namespace
{
	constexpr uint32_t WINDOW_DEFAULT_WIDTH = 1000;
	constexpr uint32_t WINDOW_DEFAULT_HEIGHT = 1000;
}

Application* Application::instance = nullptr;

Application& Application::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Application();
	}

	return *instance;
}



void Application::SetParameters(const std::filesystem::path& inExecutablePath, const std::string& inTitle)
{
	executablePath = inExecutablePath;

	// Create the main Window from which we will render the Application and set up an OpenGL Context for it
	window = std::make_unique<Window>(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, inTitle);

	// Load all OpenGL function pointers locations using GLAD, after an OpenGL Context has been set up for the current GLFW Window
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
	{
		std::cout << "ERROR::GLAD - Failed to initialise GLAD, so no OpenGL functions are available!" << std::endl;
		assert(false);
	}

	CoreEngine::GetInstance().SetUp();
}

Application::~Application()
{
	// This destructor, even if empty, ensures it will call the custom deleter of all std::unique_ptrs from this point in this source file at compile time.
	// All hold types will be fully-defined by then (while only forward-declared in header), which is a requirement of std::unique_ptr class (unlike std::shared_ptr)

	CoreEngine::GetInstance().ClearSceneForRendering();
}

void Application::Run() const
{
	CoreEngine::GetInstance().PrepareSceneForRendering();

	while (IsClosed() == false)
	{
		CoreEngine::GetInstance().Tick(IsPaused());

		window->SwapFrontAndBackBuffers();
		window->ProcessPendingEvents();
	}
}

bool Application::IsClosed() const
{
	return glfwWindowShouldClose(window->GLFWWindow);
}

void Application::Close() const
{
	glfwSetWindowShouldClose(window->GLFWWindow, true);
}

void Application::Pause(const bool inIsPaused)
{
	if (inIsPaused)
	{
		cachedSpeedFactor = speedFactor;
		speedFactor = 0.0f;
	}
	else
	{
		speedFactor = cachedSpeedFactor;
		cachedSpeedFactor = 0.0f;
	}
}

void Application::UpdateSpeed(const float inSpeedFactor)
{
	speedFactor *= inSpeedFactor;

	speedFactor = std::max(SPEED_MIN_THRESHOLD, speedFactor);
	speedFactor = std::min(SPEED_MAX_THRESHOLD, speedFactor);
}

void Application::SetScene(std::unique_ptr<Scene> scene)
{
	CoreEngine::GetInstance().SetScene(std::move(scene));
}
