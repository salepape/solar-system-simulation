#include "PerspectiveCameraController.h"

#include <glfw/glfw3.h>
#include <glm/common.hpp>

#include <cassert>
#include <iostream>

#include "InputHandler.h"
#include "Application/Application.h"
#include "Application/Window.h"
#include "Utils/Helpers.h"



PerspectiveCameraController::PerspectiveCameraController(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inZoomMaxLevel, const float inFarPlane) :
	camera(inPosition, inRotation, inZoomMaxLevel, inFarPlane),
	headlamp(inPosition),
	zoomMaxLevel(inZoomMaxLevel)
{
	zoomLeft = inZoomMaxLevel;
	mouseSensitivity = mouseMaxSensitivity;

	SetMouseInputGLFWCallback();
	SetMouseWheelInputGLFWCallback();
	SetKeyboardInputGLFWCallback();
}

void PerspectiveCameraController::ProcessUserInput(const float deltaTime)
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
	if (const auto& inputHandler = InputHandler::GetInstance();
		inputHandler.IsKeyPressed(GLFW_KEY_W) ||
		inputHandler.IsKeyPressed(GLFW_KEY_S) ||
		inputHandler.IsKeyPressed(GLFW_KEY_A) ||
		inputHandler.IsKeyPressed(GLFW_KEY_D) ||
		inputHandler.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) ||
		inputHandler.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		GetHeadlamp().UpdateHeadlight(camera);
	}

	// Modify speed
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		IncreaseTravelSpeed(2.5f);
	}
	if (InputHandler::GetInstance().IsKeyReleased(GLFW_KEY_LEFT_SHIFT))
	{
		DecreaseTravelSpeed(2.5f);
	}
}

void PerspectiveCameraController::UpdateZoomLeft(const float yOffset)
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

void PerspectiveCameraController::IncreaseTravelSpeed(const float factor)
{
	if (travelSpeed > factor * travelSpeedDefault)
	{
		return;
	}

	travelSpeed *= factor;
}

void PerspectiveCameraController::DecreaseTravelSpeed(const float factor)
{
	if (travelSpeed < travelSpeedDefault)
	{
		return;
	}

	travelSpeed *= 1.0f / factor;
}

void PerspectiveCameraController::SetMouseInputGLFWCallback()
{
	glfwSetCursorPosCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* GLFWWindow, const double xPosition, const double yPosition)
	{
		Window* const window = GLFWHelper::GetGLFWWindowPointerToUserData(GLFWWindow);
		PerspectiveCameraController* const cameraController = window->cameraController;
		if (cameraController == nullptr)
		{
			std::cout << "ERROR::PERSPECTIVE_CAMERA_CONTROLLER - No controller attached to the window..." << std::endl;
			assert(false);
		}

		PerspectiveCamera& camera = cameraController->GetCamera();

		const glm::vec2& offset = window->ComputeCursorOffset(static_cast<float>(xPosition), static_cast<float>(yPosition));
		camera.UpdateRotation(offset * cameraController->mouseSensitivity);

		cameraController->GetHeadlamp().UpdateHeadlight(camera);
	});
}

void PerspectiveCameraController::SetMouseWheelInputGLFWCallback()
{
	glfwSetScrollCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* GLFWWindow, double /*xOffset*/, double yOffset)
	{
		const Window* const window = GLFWHelper::GetGLFWWindowPointerToUserData(GLFWWindow);
		PerspectiveCameraController* const cameraController = window->cameraController;
		if (cameraController == nullptr)
		{
			std::cout << "ERROR::PERSPECTIVE_CAMERA_CONTROLLER - No controller attached to the window..." << std::endl;
			assert(false);
		}

		cameraController->UpdateZoomLeft(static_cast<float>(yOffset));
		cameraController->GetCamera().SetFovY(cameraController->zoomLeft);
	});
}

void PerspectiveCameraController::SetKeyboardInputGLFWCallback()
{
	glfwSetKeyCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* GLFWWindow, const int32_t key, const int32_t /*scanCode*/, const int32_t action, const int32_t /*mods*/)
	{
		Window* const window = GLFWHelper::GetGLFWWindowPointerToUserData(GLFWWindow);

		const auto& GetCameraController = [&window]() -> PerspectiveCameraController*
		{
			if (window == nullptr)
			{
				std::cout << "ERROR::PERSPECTIVE_CAMERA_CONTROLLER - No window found..." << std::endl;
				assert(false);
			}

			PerspectiveCameraController* const cameraController = window->cameraController;
			if (cameraController == nullptr)
			{
				std::cout << "ERROR::PERSPECTIVE_CAMERA_CONTROLLER - No controller attached to the window..." << std::endl;
				assert(false);
			}

			return cameraController;
		};

		const auto& IsReleaseActionRegistered = [&GetCameraController](const double pressTime) -> bool
		{
			const PerspectiveCameraController* const cameraController = GetCameraController();
			return Application::GetInstance().GetTime() - pressTime > cameraController->timeBeforeReleaseRegistered;
		};

		PerspectiveCameraController* const cameraController = GetCameraController();

		if (key == GLFW_KEY_SPACE)
		{
			if (action == GLFW_PRESS && cameraController->pauseStartTime == 0.0)
			{
				Application::GetInstance().Pause(true);
				cameraController->pauseStartTime = Application::GetInstance().GetTime();
			}

			if (action == GLFW_RELEASE && IsReleaseActionRegistered(cameraController->pauseStartTime))
			{
				Application::GetInstance().Pause(false);
				cameraController->pauseStartTime = 0.0;
			}
		}
		else if (key == GLFW_KEY_TAB)
		{
			if (action == GLFW_PRESS && cameraController->cursorModeStartTime == 0.0)
			{
				window->SetCursorMode(GLFW_CURSOR_NORMAL);
				cameraController->cursorModeStartTime = Application::GetInstance().GetTime();
			}

			if (action == GLFW_RELEASE && IsReleaseActionRegistered(cameraController->cursorModeStartTime))
			{
				window->SetCursorMode(GLFW_CURSOR_DISABLED);
				cameraController->cursorModeStartTime = 0.0;
			}
		}
		else if (key == GLFW_KEY_L)
		{
			if (action == GLFW_PRESS && cameraController->displayLegendStartTime == 0.0)
			{
				Application::GetInstance().DisplayLegend(true);
				cameraController->displayLegendStartTime = Application::GetInstance().GetTime();
			}

			if (action == GLFW_RELEASE && IsReleaseActionRegistered(cameraController->displayLegendStartTime))
			{
				Application::GetInstance().DisplayLegend(false);
				cameraController->displayLegendStartTime = 0.0;
			}
		}
		else if (key == GLFW_KEY_H)
		{
			cameraController->GetHeadlamp().UpdateHeadlightState(cameraController->timeBeforeReleaseRegistered, action);
		}
	});
}
