#include "Spacecraft.h"

#include "Application.h"



Spacecraft::Spacecraft(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inZoomMaxLevel, const float inFarPlane) :
	cameraController({ inPosition, inRotation, inZoomMaxLevel, inFarPlane })
{
	Application::GetInstance().GetWindow().controller = &cameraController;
}