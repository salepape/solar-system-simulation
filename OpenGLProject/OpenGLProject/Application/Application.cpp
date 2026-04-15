#include "Application.h"

#include <glad/glad.h>	// No OpenGL functions called here, just setting up GLAD
#include <glfw/glfw3.h>

#include <cassert>
#include <iostream>

#include "Rendering/ShaderLoader.h"
#include "Window.h"

Application* Application::instance = nullptr;

void Application::WindowDeleter::operator()(Window* ptr)
{
	delete ptr;
}



Application& Application::GetInstance()
{
	if (instance == nullptr)
	{
		std::cout << "ERROR::APPLICATION - A class attempts to get a reference to the Application singleton while it has not been initialised!" << std::endl;
		assert(false);
	}

	return *instance;
}

Application::Application(const std::string& inExecutablePath) :
	executablePath(inExecutablePath)
{
	window = std::unique_ptr<Window, WindowDeleter>(new Window(1000, 1000, "Solar System Simulation"));

	// Load all OpenGL function pointers locations using GLAD
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
	{
		std::cout << "ERROR::GLAD - Failed to initialise GLAD..." << std::endl;
		assert(false);
	}

	instance = this;
}

Application::~Application()
{
	window->ClearResources();
}

void Application::SetUp()
{
	ShaderLoader::BuildShaders();
}

void Application::Run()
{
	SetUp();

	// Main loop (run every frame)
	while (IsClosed() == false)
	{
		Tick();
	}
}

void Application::Tick()
{
	if (isPaused == false)
	{
		elapsedTime = static_cast<float>(GetTime());

		deltaTime = elapsedTime - lastFrameElapsedTime;
		lastFrameElapsedTime = elapsedTime;
	}

	Refresh();

	window->SwapFrontAndBackBuffers();
	window->ProcessPendingEvents();
}

bool Application::IsClosed() const
{
	return glfwWindowShouldClose(window->GLFWWindow) != 0;
}

void Application::Close() const
{
	glfwSetWindowShouldClose(window->GLFWWindow, true);
}

void Application::Pause(const bool inIsPaused)
{
	isPaused = inIsPaused;
	if (isPaused == false)
	{
		glfwSetTime(lastFrameElapsedTime);
	}
}

double Application::GetTime() const
{
	return glfwGetTime();
}

void Application::UpdateSpeed(const float inSpeedFactor)
{
	speedFactor *= inSpeedFactor;
}