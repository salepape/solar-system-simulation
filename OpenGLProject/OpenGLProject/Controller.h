#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Camera.h"
#include "SpotLight.h"

#include <glm/vec3.hpp>



// Camera controller that processes keyboard input, manages a spotlight, and calculates camera Euler angles
class Controller
{
public:
	Controller(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inZoomMaxLevel, const float inFarPlane);

	void ProcessKeyboardInput(const float deltaTime);

	Camera& GetCamera() { return camera; }
	float GetZoomLeft() const { return zoomLeft; }

	// Turn on/off reflection params rather than creating/deleting a heap-allocated SpotLight instance
	void SetFlashLightState(const bool isActive);
	void UpdateFlashLight();

private:
	Camera camera;

	SpotLight flashLight;

	// [in km/s]
	float travelSpeed{ 20.0f };
	float travelSpeedDefault{ 20.0f };

	// [dimensionless]
	float mouseSensitivity{ 0.05f };
	float mouseMaxSensitivity{ 0.05f };

	// [in degrees]
	float zoomLeft{ 45.0f };
	float zoomMaxLevel{ 45.0f };

	// Cache used to avoid detecting the button release happening after a button press if quick
	double pauseStartTime{ 0.0 };
	double displayLegendStartTime{ 0.0 };
	double cursorModeStartTime{ 0.0 };
	double flashLightStartTime{ 0.0 };

	// Allow to not register a release action if the time that separates it from the preceeding press action is below this one, considered too small
	double timeBeforeReleaseRegistered{ 1.0 };

	// Process input received from a mouse scroll-wheel event (vertical wheel-axis to be considered only)
	void UpdateZoomLeft(const float yOffset);

	void IncreaseTravelSpeed(const float factor);
	void DecreaseTravelSpeed(const float factor);

	// Call corresponding GLFW callback functions
	void Callback_DetectMouseInput();
	void Callback_DetectMouseWheelInput();
	void Callback_DetectKeyboardInput();
};



#endif // CONTROLLER_H