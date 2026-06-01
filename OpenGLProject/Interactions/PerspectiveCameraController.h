#ifndef PERSPECTIVE_CAMERA_CONTROLLER_H
#define PERSPECTIVE_CAMERA_CONTROLLER_H

#include <glm/vec3.hpp>

#include "Cameras/PerspectiveCamera.h"
#include "Headlamp.h"
#include "Scene/Transform.h"



// Detect and process all keyboard and mouse user input (some of it registered based on callback)
class PerspectiveCameraController
{
public:
	PerspectiveCameraController(const glm::vec3& inPosition, const EulerAngles& inRotation, const float inZoomMaxLevel, const float inFarPlane);

	// Detect input where second keyboard key press switch back to default state
	void ProcessUserInput(const float deltaTime);

	PerspectiveCamera& GetCamera() { return camera; }
	Headlamp& GetHeadlamp() { return headlamp; }

	// [dimensionless]
	float mouseMaxSensitivity{ 0.05f };
	float mouseSensitivity{ 0.05f };

private:
	PerspectiveCamera camera;
	Headlamp headlamp;

	// [in degrees]
	float zoomMaxLevel{ 45.0f };
	float zoomLevelLeft{ 45.0f };

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

	// Detect input where second keyboard key press switch back to default state
	void SetMouseInputGLFWCallback();
	void SetMouseWheelInputGLFWCallback();
	void SetKeyboardInputGLFWCallback();
};



#endif // PERSPECTIVE_CAMERA_CONTROLLER_H
