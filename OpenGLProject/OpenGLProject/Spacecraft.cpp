#include "Spacecraft.h"

#include "Application.h"
#include "Window.h"



Spacecraft::Spacecraft(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inZoomMaxLevel, const float inFarPlane) :
	cameraController({ inPosition, inRotation, inZoomMaxLevel, inFarPlane })
{
	Window& currentWindow = Application::GetInstance().GetWindow();
	currentWindow.cameraController = &cameraController;
}

void Spacecraft::SetInitialTransform(const glm::vec3& inPosition, const glm::vec3& inRotation)
{
	cameraController.GetCamera().SetInitialTransform(inPosition, inRotation);
}