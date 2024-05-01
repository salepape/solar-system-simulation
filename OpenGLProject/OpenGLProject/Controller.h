#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Camera.h"

#include <glm/vec3.hpp>



// Controller class that processes input and calculates its Euler angles, vectors and matrices
class Controller
{
public:
	Controller(const glm::vec3& position, const float inZoomMaxLevel = 45.0f);

	// Process controller input received from any keyboard-like input system
	void ProcessInput(const float deltaTime);

	// Process input received from a mouse input system
	void ProcessMouseMovement(float xoffset, float yoffset);

	// Process input received from a mouse scroll-wheel event (vertical wheel-axis to be considered only)
	void ProcessMouseScroll(const float yoffset);

	void IncreaseSpeed(const float factor);
	void DecreaseSpeed(const float factor);

	const Camera& GetCamera() const { return camera; }

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
};



#endif // CONTROLLER_H