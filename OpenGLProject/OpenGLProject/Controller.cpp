#include "Controller.h"

#include <glfw3.h>
#include <glm/common.hpp>
#include <iostream>

#include "Application.h"
#include "InputHandler.h"
#include "Window.h"



Controller::Controller(const glm::vec3& inPosition, const float inZoomMaxLevel) : camera({ inPosition }), zoomMaxLevel(inZoomMaxLevel)
{
	zoomLeft = inZoomMaxLevel;

	Callback_SetCursorPosition();
	Callback_SetScroll();
}

void Controller::ProcessInput(const float deltaTime)
{
	// Quit the simulation
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		Application::GetInstance().Close();
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

	// Designed for AZERTY keyboards with corresponding QWERTY GLFW_KEYs
	const float distance = travelSpeed * deltaTime;
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_W))
	{
		camera.UpdateForwardPosition(distance);
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_S))
	{
		camera.UpdateForwardPosition(-distance);
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_A))
	{
		camera.UpdateRightPosition(-distance);
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_D))
	{
		camera.UpdateRightPosition(distance);
	}
	if (InputHandler::GetInstance().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		camera.UpdateUpPosition(distance);
	}
	if (InputHandler::GetInstance().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		camera.UpdateUpPosition(-distance);
	}

	// Modify controller speed
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_X))
	{
		IncreaseSpeed(2.0f);
	}
	if (InputHandler::GetInstance().IsKeyReleased(GLFW_KEY_X))
	{
		DecreaseSpeed(2.0f);
	}
}

void Controller::UpdateZoomLeft(const float yOffset)
{
	constexpr float zoomMinLevel = 1.0f;
	if (zoomLeft >= zoomMinLevel && zoomLeft <= zoomMaxLevel)
	{
		zoomLeft -= yOffset;
	}

	zoomLeft = glm::clamp(zoomLeft, zoomMinLevel, zoomMaxLevel);
}

void Controller::IncreaseSpeed(const float factor)
{
	if (travelSpeed > factor * travelSpeedDefault)
	{
		return;
	}

	travelSpeed *= factor;
}

void Controller::DecreaseSpeed(const float factor)
{
	if (travelSpeed < travelSpeedDefault)
	{
		return;
	}

	travelSpeed *= 1.0f / factor;
}

void Controller::Callback_SetCursorPosition()
{
	glfwSetCursorPosCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* window, const double xPosition, const double yPosition)
	{
		// Access contextual data from within GLFWwindow callback
		auto* const currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (currentWindow == nullptr)
		{
			std::cout << "ERROR::CONTROLLER - Failed to cast glfwGetWindowUserPointer()" << std::endl;
			return;
		}

		const auto& offset = currentWindow->GetOffsetFromLastCursorPosition(static_cast<float>(xPosition), static_cast<float>(yPosition));

		if (auto* const currentController = currentWindow->controller)
		{
			currentController->GetCamera().UpdateRotation(offset * currentController->mouseSensitivity);
		}
	});
}

void Controller::Callback_SetScroll()
{
	glfwSetScrollCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		// Access contextual data from within GLFWwindow callback
		auto* const currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (currentWindow == nullptr)
		{
			std::cout << "ERROR::CONTROLLER - Failed to cast glfwGetWindowUserPointer()" << std::endl;
			return;
		}

		if (auto* const currentController = currentWindow->controller)
		{
			currentController->UpdateZoomLeft(static_cast<float>(yOffset));
		}
	});
}