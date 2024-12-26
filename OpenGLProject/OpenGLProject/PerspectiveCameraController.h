#ifndef PERSPECTIVE_CAMERA_CONTROLLER_H
#define PERSPECTIVE_CAMERA_CONTROLLER_H

#include <glm/vec3.hpp>

#include "PerspectiveCamera.h"
#include "Headlamp.h"



// Camera controller that processes all keyboard user input
class PerspectiveCameraController
{
public:
	PerspectiveCameraController(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inZoomMaxLevel, const float inFarPlane);

	void ProcessKeyboardInput(const float deltaTime);

	PerspectiveCamera& GetCamera() { return camera; }
	Headlamp& GetHeadlamp() { return headlamp; }

	// [dimensionless]
	float mouseMaxSensitivity{ 0.05f };
	float mouseSensitivity{ 0.05f };

	// Allow to not register a release action if the time that separates it from the preceeding press action is below this one, considered too small
	double timeBeforeReleaseRegistered{ 1.0 };

private:
	PerspectiveCamera camera;
	Headlamp headlamp;

	// [in degrees]
	float zoomMaxLevel{ 45.0f };
	float zoomLeft{ 45.0f };

	// [in km/s]
	float travelSpeedDefault{ 20.0f };
	float travelSpeed{ 20.0f };

	// Cache used to avoid detecting the button release happening after a button press if quick
	double pauseStartTime{ 0.0 };
	double displayLegendStartTime{ 0.0 };
	double cursorModeStartTime{ 0.0 };

	// Process input received from a mouse scroll-wheel event (vertical wheel-axis to be considered only)
	void UpdateZoomLeft(const float yOffset);

	void IncreaseTravelSpeed(const float factor);
	void DecreaseTravelSpeed(const float factor);

	// Call corresponding GLFW callback functions
	void Callback_DetectMouseInput();
	void Callback_DetectMouseWheelInput();
	void Callback_DetectKeyboardInput();
};



#endif // PERSPECTIVE_CAMERA_CONTROLLER_H