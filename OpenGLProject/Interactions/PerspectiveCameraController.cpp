#include "PerspectiveCameraController.h"

#include <glfw/glfw3.h>
#include <glm/common.hpp>
#include <glm/trigonometric.hpp>

#include <cassert>
#include <iostream>

#include "Application/Application.h"
#include "Application/ApplicationControls.h"
#include "Application/Window.h"
#include "InputHandler.h"
#include "Utils/Helpers.h"



PerspectiveCameraController::PerspectiveCameraController(const glm::vec3& inPosition, const EulerAngles& inRotation, const float inZoomMaxLevel, const float inFarPlane) :
	camera(inPosition, inRotation, inZoomMaxLevel, inFarPlane),
	headlamp(inPosition),
	zoomMaxLevel(inZoomMaxLevel)
{
	zoomLevelLeft = inZoomMaxLevel;
	mouseSensitivity = mouseMaxSensitivity;

	SetMouseInputGLFWCallback();
	SetMouseWheelInputGLFWCallback();
	SetKeyboardInputGLFWCallback();
}

void PerspectiveCameraController::ProcessUserInput(const float deltaTime)
{
	// Reset user's position in Scene
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_R))
	{
		camera.ResetTransform();
	}

	// Designed for AZERTY keyboards with corresponding QWERTY GLFW_KEYs
	const float deltaDistance = travelSpeed * deltaTime;
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_W))
	{
		camera.Translate(deltaDistance, camera.GetCameraForward());
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_S))
	{
		camera.Translate(deltaDistance, -camera.GetCameraForward());
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_A))
	{
		camera.Translate(deltaDistance, -camera.GetCameraRight());
	}
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_D))
	{
		camera.Translate(deltaDistance, camera.GetCameraRight());
	}
	if (InputHandler::GetInstance().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		camera.Translate(deltaDistance, camera.GetCameraUp());
	}
	if (InputHandler::GetInstance().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		camera.Translate(deltaDistance, -camera.GetCameraUp());
	}

	// Update lighting projected by headlight
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

	// Modify user speed
	if (InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_LEFT_SHIFT) ||
		InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
	{
		IncreaseTravelSpeed(2.5f);
	}
	if (InputHandler::GetInstance().IsKeyReleased(GLFW_KEY_LEFT_SHIFT) ||
		InputHandler::GetInstance().IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
	{
		DecreaseTravelSpeed(2.5f);
	}
}

void PerspectiveCameraController::UpdateZoomLeft(const float yOffset)
{
	constexpr float zoomMinLevel = 1.0f;
	if (zoomLevelLeft >= zoomMinLevel && zoomLevelLeft <= zoomMaxLevel)
	{
		zoomLevelLeft -= yOffset;
		mouseSensitivity -= yOffset * 1.0f / 1000;
	}

	zoomLevelLeft = glm::clamp(zoomLevelLeft, zoomMinLevel, zoomMaxLevel);
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

		PerspectiveCamera& camera = cameraController->GetCamera();

		const glm::vec2& offset = window->ComputeCursorOffset(static_cast<float>(xPosition), static_cast<float>(yPosition));
		camera.UpdateRotation(EulerAngles{ 0.0f, glm::radians(offset.x * cameraController->mouseSensitivity), glm::radians(offset.y * cameraController->mouseSensitivity) });

		cameraController->GetHeadlamp().UpdateHeadlight(camera);
	});
}

void PerspectiveCameraController::SetMouseWheelInputGLFWCallback()
{
	glfwSetScrollCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* GLFWWindow, double /*xOffset*/, double yOffset)
	{
		const Window* const window = GLFWHelper::GetGLFWWindowPointerToUserData(GLFWWindow);
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

		cameraController->UpdateZoomLeft(static_cast<float>(yOffset));
		cameraController->GetCamera().SetFovY(cameraController->zoomLevelLeft);
	});
}

void PerspectiveCameraController::SetKeyboardInputGLFWCallback()
{
	// Warning: any further call to this GLFW function will overwrite any existing lambda callback parameter previously set!
	glfwSetKeyCallback(Application::GetInstance().GetWindow().GLFWWindow, [](GLFWwindow* GLFWWindow, const int32_t key, const int32_t /*scanCode*/, const int32_t action, const int32_t /*mods*/)
	{
		Window* const window = GLFWHelper::GetGLFWWindowPointerToUserData(GLFWWindow);
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

		const auto& IsReleaseActionRegistered = [](const float pressTime) -> bool
		{
			return Application::GetInstance().GetElapsedTime() - pressTime > ApplicationControls::KEY_RELEASE_SENSITIVITY;
		};

		// Pause simulation
		if (key == GLFW_KEY_SPACE)
		{
			if (action == GLFW_PRESS && cameraController->pauseStartTime == 0.0f)
			{
				Application::GetInstance().Pause(true);
				cameraController->pauseStartTime = Application::GetInstance().GetElapsedTime();
			}

			if (action == GLFW_RELEASE && IsReleaseActionRegistered(cameraController->pauseStartTime))
			{
				Application::GetInstance().Pause(false);
				cameraController->pauseStartTime = 0.0f;
			}
		}
		// Switch application cursor mode
		else if (key == GLFW_KEY_TAB)
		{
			if (action == GLFW_PRESS && cameraController->cursorModeStartTime == 0.0f)
			{
				window->SetCursorMode(GLFW_CURSOR_NORMAL);
				cameraController->cursorModeStartTime = Application::GetInstance().GetElapsedTime();
			}

			if (action == GLFW_RELEASE && IsReleaseActionRegistered(cameraController->cursorModeStartTime))
			{
				window->SetCursorMode(GLFW_CURSOR_DISABLED);
				cameraController->cursorModeStartTime = 0.0f;
			}
		}
		// Display legend on each celestial body of the simulation
		else if (key == GLFW_KEY_L)
		{
			if (action == GLFW_PRESS && cameraController->displayLegendStartTime == 0.0f)
			{
				Application::GetInstance().DisplayLegend(true);
				cameraController->displayLegendStartTime = Application::GetInstance().GetElapsedTime();
			}

			if (action == GLFW_RELEASE && IsReleaseActionRegistered(cameraController->displayLegendStartTime))
			{
				Application::GetInstance().DisplayLegend(false);
				cameraController->displayLegendStartTime = 0.0f;
			}
		}
		// @todo - Spot Light does not disappear at second 'H' key press
		else if (key == GLFW_KEY_H)
		{
			cameraController->GetHeadlamp().UpdateHeadlightState(action);
		}
	});
}
