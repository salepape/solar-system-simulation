#include "Controller.h"

#include <glfw3.h>
#include <glm/common.hpp>
#include <iostream>

#include "Application.h"
#include "InputHandler.h"
#include "Window.h"



Controller::Controller(const glm::vec3& inPosition, const float inZoomMaxLevel, const float inFarPlane, const std::vector<uint32_t>& shaderIDs) :
	camera({ inPosition, inZoomMaxLevel, inFarPlane, shaderIDs }), zoomMaxLevel(inZoomMaxLevel)
{
	zoomLeft = inZoomMaxLevel;

	Callback_SetCursorPosition();
	Callback_SetScroll();
	Callback_SetPause();
}

void Controller::ProcessInput(const float deltaTime)
{
	// Quit the simulation
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		Application::GetInstance().Close();
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
			std::cout << "ERROR::CONTROLLER - Failed to cast glfwGetWindowUserPointer()!" << std::endl;
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
	glfwSetScrollCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* window, double /*xOffset*/, double yOffset)
	{
		// Access contextual data from within GLFWwindow callback
		auto* const currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (currentWindow == nullptr)
		{
			std::cout << "ERROR::CONTROLLER - Failed to cast glfwGetWindowUserPointer()!" << std::endl;
			return;
		}

		if (auto* const currentController = currentWindow->controller)
		{
			currentController->UpdateZoomLeft(static_cast<float>(yOffset));
			currentController->GetCamera().SetFovY(currentController->zoomLeft);
		}
	});
}

void Controller::Callback_SetPause()
{
	glfwSetKeyCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* window, const int32_t key, const int32_t /*scanCode*/, const int32_t action, const int32_t /*mods*/)
	{
		auto GetCurrentController = [&window]() -> Controller*
		{
			// Access contextual data from within GLFWwindow callback
			auto* const currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
			if (currentWindow == nullptr)
			{
				std::cout << "ERROR::CONTROLLER - Failed to cast glfwGetWindowUserPointer()" << std::endl;
			}

			if (auto* const currentController = currentWindow->controller)
			{
				return currentController;
			}

			return nullptr;
		};

		if (key == GLFW_KEY_SPACE)
		{
			const auto& currentController = GetCurrentController();
			if (action == GLFW_PRESS && currentController && currentController->pauseStartTime == 0.0)
			{
				Application::GetInstance().Pause(true);
				currentController->pauseStartTime = Application::GetInstance().GetTime();
			}

			if (action == GLFW_RELEASE && currentController && Application::GetInstance().GetTime() - currentController->pauseStartTime > currentController->detectedButtonReleaseMinDuration)
			{
				Application::GetInstance().Pause(false);
				currentController->pauseStartTime = 0.0;
			}
		}
		else if (key == GLFW_KEY_TAB)
		{
			const auto& currentController = GetCurrentController();
			if (action == GLFW_PRESS && currentController && currentController->cursorModeStartTime == 0.0)
			{
				Application::GetInstance().GetWindow().SetCursorMode(GLFW_CURSOR_NORMAL);
				currentController->cursorModeStartTime = Application::GetInstance().GetTime();
			}

			if (action == GLFW_RELEASE && currentController && Application::GetInstance().GetTime() - currentController->cursorModeStartTime > currentController->detectedButtonReleaseMinDuration)
			{
				Application::GetInstance().GetWindow().SetCursorMode(GLFW_CURSOR_DISABLED);
				currentController->cursorModeStartTime = 0.0;
			}
		}
		else if (key == GLFW_KEY_L)
		{
			const auto& currentController = GetCurrentController();
			if (action == GLFW_PRESS && currentController && currentController->displayLegendStartTime == 0.0)
			{
				Application::GetInstance().DisplayLegend(true);
				currentController->displayLegendStartTime = Application::GetInstance().GetTime();
			}

			if (action == GLFW_RELEASE && currentController && Application::GetInstance().GetTime() - currentController->displayLegendStartTime > currentController->detectedButtonReleaseMinDuration)
			{
				Application::GetInstance().DisplayLegend(false);
				currentController->displayLegendStartTime = 0.0;
			}
		}
	});
}