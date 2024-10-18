#include "Controller.h"

#include <glfw3.h>
#include <glm/common.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>

#include "Application.h"
#include "InputHandler.h"
#include "Window.h"



Controller::Controller(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inZoomMaxLevel, const float inFarPlane) :
	camera({ inPosition, inRotation, inZoomMaxLevel, inFarPlane }), zoomMaxLevel(inZoomMaxLevel),
	flashLight(inPosition, glm::vec3(0.0f, 0.0f, -1.0f), { glm::vec3(0.2f), glm::vec3(0.8f), glm::vec3(1.0f) }, { 1.0f, 0.007f, 0.0002f }, { glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)) })
{
	zoomLeft = inZoomMaxLevel;
	mouseSensitivity = mouseMaxSensitivity;

	Callback_DetectMouseInput();
	Callback_DetectMouseWheelInput();
	Callback_DetectKeyboardInput();
}

void Controller::ProcessKeyboardInput(const float deltaTime)
{
	// Quit the simulation
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		Application::GetInstance().Close();
	}

	// Speed up/Slow down the simulation
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_UP))
	{
		Application::GetInstance().UpdateSpeed(2.0f);
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_DOWN))
	{
		Application::GetInstance().UpdateSpeed(0.5f);
	}

	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_R))
	{
		camera.ResetTransform();
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
	if (const auto inputHandler = InputHandler::GetInstance();
		flashLightStartTime > 0.0 && (
			inputHandler.IsKeyPressed(GLFW_KEY_W) ||
			inputHandler.IsKeyPressed(GLFW_KEY_S) ||
			inputHandler.IsKeyPressed(GLFW_KEY_A) ||
			inputHandler.IsKeyPressed(GLFW_KEY_D) ||
			inputHandler.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) ||
			inputHandler.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)))
	{
		UpdateFlashLight();
	}

	// Modify controller speed
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_X))
	{
		IncreaseTravelSpeed(2.5f);
	}
	if (InputHandler::GetInstance().IsKeyReleased(GLFW_KEY_X))
	{
		DecreaseTravelSpeed(2.5f);
	}
}

void Controller::UpdateZoomLeft(const float yOffset)
{
	constexpr float zoomMinLevel = 1.0f;
	if (zoomLeft >= zoomMinLevel && zoomLeft <= zoomMaxLevel)
	{
		zoomLeft -= yOffset;
		mouseSensitivity -= yOffset * 1.0f / 1000;
	}

	zoomLeft = glm::clamp(zoomLeft, zoomMinLevel, zoomMaxLevel);
	mouseSensitivity = glm::clamp(mouseSensitivity, mouseMaxSensitivity * 1.0f / 10, mouseMaxSensitivity);
}

void Controller::IncreaseTravelSpeed(const float factor)
{
	if (travelSpeed > factor * travelSpeedDefault)
	{
		return;
	}

	travelSpeed *= factor;
}

void Controller::DecreaseTravelSpeed(const float factor)
{
	if (travelSpeed < travelSpeedDefault)
	{
		return;
	}

	travelSpeed *= 1.0f / factor;
}

void Controller::SetFlashLightState(const bool isActive)
{
	flashLight.SetIsCameraFlashLightFUniform(isActive);
}

void Controller::UpdateFlashLight()
{
	flashLight.SetLightPositionFUniform(camera.GetPosition());
	flashLight.SetLightDirectionFUniform(camera.GetForward());
}

void Controller::Callback_DetectMouseInput()
{
	glfwSetCursorPosCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* GLFWWindow, const double xPosition, const double yPosition)
	{
		// Access contextual data from within GLFWWindow callback
		auto* const window = static_cast<Window*>(glfwGetWindowUserPointer(GLFWWindow));
		if (window == nullptr)
		{
			std::cout << "ERROR::CONTROLLER - Failed to cast glfwGetWindowUserPointer()!" << std::endl;
			return;
		}

		const auto controller = window->controller;
		if (controller == nullptr)
		{
			std::cout << "ERROR::CONTROLLER - No controller attached to the window..." << std::endl;
			return;
		}

		const auto& offset = window->ComputeCursorOffset(static_cast<float>(xPosition), static_cast<float>(yPosition));
		controller->GetCamera().UpdateRotation(offset * controller->mouseSensitivity);

		if (controller->flashLightStartTime > 0.0)
		{
			controller->UpdateFlashLight();
		}
	});
}

void Controller::Callback_DetectMouseWheelInput()
{
	glfwSetScrollCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* GLFWWindow, double /*xOffset*/, double yOffset)
	{
		// Access contextual data from within GLFWWindow callback
		const auto* const window = static_cast<Window*>(glfwGetWindowUserPointer(GLFWWindow));
		if (window == nullptr)
		{
			std::cout << "ERROR::CONTROLLER - Failed to cast glfwGetWindowUserPointer()!" << std::endl;
			return;
		}

		const auto controller = window->controller;
		if (controller == nullptr)
		{
			std::cout << "ERROR::CONTROLLER - No controller attached to the window..." << std::endl;
			return;
		}

		controller->UpdateZoomLeft(static_cast<float>(yOffset));
		controller->GetCamera().SetFovY(controller->zoomLeft);
	});
}

void Controller::Callback_DetectKeyboardInput()
{
	glfwSetKeyCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* GLFWWindow, const int32_t key, const int32_t /*scanCode*/, const int32_t action, const int32_t /*mods*/)
	{
		auto GetWindow = [&GLFWWindow]() -> Window*
		{
			// Access contextual data from within GLFWWindow callback
			auto* const window = static_cast<Window*>(glfwGetWindowUserPointer(GLFWWindow));
			if (window == nullptr)
			{
				std::cout << "ERROR::CONTROLLER - Failed to cast glfwGetWindowUserPointer()" << std::endl;
				return nullptr;
			}

			return window;
		};

		const auto GetController = [&GLFWWindow, &GetWindow]() -> std::shared_ptr<Controller>
		{
			const auto* const window = GetWindow();
			if (window == nullptr)
			{
				return nullptr;
			}

			const auto controller = window->controller;
			if (controller == nullptr)
			{
				std::cout << "ERROR::CONTROLLER - No controller attached to the window..." << std::endl;
			}

			return controller;
		};

		const auto IsReleaseActionRegistered = [&GetController](const double pressTime)
		{
			const auto controller = GetController();
			return Application::GetInstance().GetTime() - pressTime > controller->timeBeforeReleaseRegistered;
		};

		if (key == GLFW_KEY_SPACE)
		{
			const auto& controller = GetController();
			if (controller == nullptr)
			{
				return;
			}

			if (action == GLFW_PRESS && controller->pauseStartTime == 0.0)
			{
				Application::GetInstance().Pause(true);
				controller->pauseStartTime = Application::GetInstance().GetTime();
			}

			if (action == GLFW_RELEASE && IsReleaseActionRegistered(controller->pauseStartTime))
			{
				Application::GetInstance().Pause(false);
				controller->pauseStartTime = 0.0;
			}
		}
		else if (key == GLFW_KEY_TAB)
		{
			const auto& controller = GetController();
			if (controller == nullptr)
			{
				return;
			}

			if (action == GLFW_PRESS && controller->cursorModeStartTime == 0.0)
			{
				GetWindow()->SetCursorMode(GLFW_CURSOR_NORMAL);
				controller->cursorModeStartTime = Application::GetInstance().GetTime();
			}

			if (action == GLFW_RELEASE && IsReleaseActionRegistered(controller->cursorModeStartTime))
			{
				GetWindow()->SetCursorMode(GLFW_CURSOR_DISABLED);
				controller->cursorModeStartTime = 0.0;
			}
		}
		else if (key == GLFW_KEY_L)
		{
			const auto& controller = GetController();
			if (controller == nullptr)
			{
				return;
			}

			if (action == GLFW_PRESS && controller->displayLegendStartTime == 0.0)
			{
				Application::GetInstance().DisplayLegend(true);
				controller->displayLegendStartTime = Application::GetInstance().GetTime();
			}

			if (action == GLFW_RELEASE && IsReleaseActionRegistered(controller->displayLegendStartTime))
			{
				Application::GetInstance().DisplayLegend(false);
				controller->displayLegendStartTime = 0.0;
			}
		}
		else if (key == GLFW_KEY_F)
		{
			const auto& controller = GetController();
			if (controller == nullptr)
			{
				return;
			}

			if (action == GLFW_PRESS && controller->flashLightStartTime == 0.0)
			{
				controller->SetFlashLightState(true);
				controller->flashLightStartTime = Application::GetInstance().GetTime();
			}

			if (action == GLFW_RELEASE && IsReleaseActionRegistered(controller->flashLightStartTime))
			{
				controller->SetFlashLightState(false);
				controller->flashLightStartTime = 0.0;
			}
		}
	});
}