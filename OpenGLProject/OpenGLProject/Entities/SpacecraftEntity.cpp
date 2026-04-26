#include "SpacecraftEntity.h"

#include "Application/Application.h"
#include "Application/Window.h"



SpacecraftEntity::SpacecraftEntity(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inZoomMaxLevel, const float inFarPlane) : SceneEntity("Spacecraft"),
cameraController(inPosition, inRotation, inZoomMaxLevel, inFarPlane)
{
	Window& currentWindow = Application::GetInstance().GetWindow();
	currentWindow.cameraController = &cameraController;
}

void SpacecraftEntity::SetInitialTransform(const glm::vec3& inPosition, const glm::vec3& inRotation)
{
	cameraController.GetCamera().SetInitialTransform(inPosition, inRotation);
}
