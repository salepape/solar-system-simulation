#include "Application.h"

#include <iostream>
#include <glad.h>
#include <glfw3.h>

Application* Application::instance = nullptr;



Application::Application()
{
	window = std::make_unique<Window>(1000, 1000, "Solar System Simulation");

	// Load all OpenGL function pointers locations using GLAD
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
	{
		std::cout << "ERROR::GLAD - Failed to initialise GLAD" << std::endl;
	}

	instance = this;
}

void Application::Run()
{
	SetUp();

	// Main loop (run every frame)
	while (IsClosed() == false)
	{
		Tick();
	}

	Terminate();
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

void Application::Terminate()
{
	window->ClearResources();
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

double Application::GetTime()
{
	return glfwGetTime();
}

void Application::UpdateSpeed(const float inSpeedFactor)
{
	speedFactor *= inSpeedFactor;
}