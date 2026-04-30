#ifndef PERSPECTIVE_CAMERA_CONTROLLER_H
#define PERSPECTIVE_CAMERA_CONTROLLER_H

#include <glm/vec3.hpp>

#include "Cameras/PerspectiveCamera.h"
#include "Headlamp.h"



// Detect and process all keyboard and mouse user input
class PerspectiveCameraController
{
public:
	PerspectiveCameraController(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inZoomMaxLevel, const float inFarPlane);

	// Trigger code logic according to the keyboard key/mouse button pressed or released (see 'Controls' section of README)
	void ProcessUserInput(const float deltaTime);

	PerspectiveCamera& GetCamera() { return camera; }
	Headlamp& GetHeadlamp() { return headlamp; }

	// [dimensionless]
	float mouseMaxSensitivity{ 0.05f };
	float mouseSensitivity{ 0.05f };

	// Keyboard key release action will not be registered if press action has happened at a delta time equal/lower to this value
	float keyReleaseSensitivity{ 1.0f };

private:
	PerspectiveCamera camera;
	Headlamp headlamp;

	// [in degrees]
	float zoomMaxLevel{ 45.0f };
	float zoomLeft{ 45.0f };

	// [in km/s]
	float travelSpeedDefault{ 20.0f };
	float travelSpeed{ 20.0f };

	// Time caching used to avoid detecting when a keyboard key has been released while it has been pressed just before
	float pauseStartTime{ 0.0f };
	float displayLegendStartTime{ 0.0f };
	float cursorModeStartTime{ 0.0f };

	// Process input received from a mouse scroll-wheel event (vertical wheel-axis to be considered only)
	void UpdateZoomLeft(const float yOffset);

	void IncreaseTravelSpeed(const float factor);
	void DecreaseTravelSpeed(const float factor);

	// Detect user input relative to any action occurring from within the bounds of the current GLFW Window
	void SetMouseInputGLFWCallback();
	void SetMouseWheelInputGLFWCallback();
	void SetKeyboardInputGLFWCallback();
};



#endif // PERSPECTIVE_CAMERA_CONTROLLER_H
