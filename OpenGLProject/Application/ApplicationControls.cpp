#include "ApplicationControls.h"

#include <glfw/glfw3.h>

#include "Application/Application.h"
#include "Interactions/InputHandler.h"



void ApplicationControls::ProcessUserInput()
{
	// Quit the Application
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		Application::GetInstance().Close();
	}

	// Speed up/Slow down the simulation run by the Application (only if it has not reached thresholds and has not been paused)
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_UP) &&
		Application::GetInstance().IsPaused() == false && Application::GetInstance().IsMaxSpeed() == false)
	{
		Application::GetInstance().UpdateSpeed(2.0f);
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_DOWN) &&
		Application::GetInstance().IsPaused() == false && Application::GetInstance().IsMinSpeed() == false)
	{
		Application::GetInstance().UpdateSpeed(0.5f);
	}
}
