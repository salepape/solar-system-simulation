#include "Application.h"

#include <glad/glad.h>	// No OpenGL functions called here, just setting up GLAD
#include <glfw/glfw3.h>

#include <algorithm>	// for std::min and std::max
#include <cassert>
#include <iostream>
#include <utility>

#include "Application/ApplicationControls.h"
#include "Rendering/Renderer.h"
#include "Rendering/ShaderLoader.h"
#include "Rendering/GlyphLoader.h"
#include "Scene/Scene.h"
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
		std::cout << "ERROR::APPLICATION - A class attempts to get a reference to the Application singleton while it has not been initialised!" << std::endl;
		assert(false);
	}

	return *instance;
}



Application::Application(const std::filesystem::path& inExecutablePath, const std::string& inTitle) :
	executablePath(inExecutablePath)
{
	// Singleton instance set up here
	instance = this;

	// Create the main Window from which we will render the Application and set up an OpenGL Context for it
	window = std::make_unique<Window>(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, inTitle);

	// Load all OpenGL function pointers locations using GLAD, after an OpenGL Context has been set up for the current GLFW Window
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
	{
		std::cout << "ERROR::GLAD - Failed to initialise GLAD, so no OpenGL functions are available!" << std::endl;
		assert(false);
	}

	ShaderLoader::BuildShaders();
	GlyphLoader::LoadASCIICharacters();
}

Application::~Application()
{
	// This destructor, even if empty, ensures it will call the custom deleter of all std::unique_ptrs from this point in this source file at compile time.
	// All hold types will be fully-defined by then (while only forward-declared in header), which is a requirement of std::unique_ptr class (unlike std::shared_ptr)
}

void Application::Run()
{
	while (IsClosed() == false)
	{
		Tick();
	}
}

void Application::Tick()
{
	// Delta time should always be computed, regardless of pause state
	if (IsPaused() == false)
	{
		elapsedPlayTime = GetElapsedTime() - elapsedPauseTime;
		deltaTime = elapsedPlayTime - lastFrameElapsedPlayTime;
		lastFrameElapsedPlayTime = elapsedPlayTime;
	}
	else
	{
		elapsedPauseTime = GetElapsedTime() - elapsedPlayTime;
		deltaTime = elapsedPauseTime - lastFrameElapsedPauseTime;
		lastFrameElapsedPauseTime = elapsedPauseTime;
	}

	Refresh();
}

void Application::Refresh()
{
	Renderer::ClearBufferTargets();

	ApplicationControls::ProcessUserInput();

	if (scene != nullptr)
	{
		scene->Update(deltaTime);
	}

	window->SwapFrontAndBackBuffers();
	window->ProcessPendingEvents();
}

bool Application::IsClosed() const
{
	return glfwWindowShouldClose(window->GLFWWindow);
}

void Application::Close() const
{
	glfwSetWindowShouldClose(window->GLFWWindow, true);
}

float Application::GetElapsedTime() const
{
	return static_cast<float>(glfwGetTime());
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

void Application::AddScene(std::unique_ptr<Scene> inScene)
{
	scene = std::move(inScene);
}
