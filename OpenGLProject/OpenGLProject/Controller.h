#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Camera.h"

#include <glm/vec3.hpp>



// Controller class that processes input and calculates its Euler angles, vectors and matrices
class Controller
{
public:
	Controller(const glm::vec3& position, const float inZoomMaxLevel, const std::vector<unsigned int>& shaderIDs);

	// Process controller input received from any keyboard-like input system
	void ProcessInput(const float deltaTime);

	Camera& GetCamera() { return camera; }

	float GetZoomLeft() const { return zoomLeft; }

private:
	Camera camera;

	// [in km/s]
	float travelSpeed{ 20.0f };
	float travelSpeedDefault{ 20.0f };

	// [dimensionless]
	float mouseSensitivity{ 0.1f };

	// [in degrees]
	float zoomLeft{ 0.0f };
	float zoomMaxLevel{ 45.0f };

	// Process input received from a mouse scroll-wheel event (vertical wheel-axis to be considered only)
	void UpdateZoomLeft(const float yoffset);

	void IncreaseSpeed(const float factor);
	void DecreaseSpeed(const float factor);

	// Detect if any mouse movement is masde and react accordingly
	void Callback_SetCursorPosition();

	// Detect if any mouse wheel movement is made and react accordingly
	void Callback_SetScroll();
};



#endif // CONTROLLER_H