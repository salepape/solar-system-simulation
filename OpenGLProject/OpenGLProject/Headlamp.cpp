#include "Headlamp.h"

#include <glfw3.h>
#include <glm/trigonometric.hpp>

#include "Application.h"
#include "Camera.h"
#include "Controller.h"



Headlamp::Headlamp(const glm::vec3& inPosition) :
	headlight(inPosition, glm::vec3(0.0f, 0.0f, -1.0f), { glm::vec3(0.2f), glm::vec3(0.8f), glm::vec3(1.0f) }, { 1.0f, 0.007f, 0.0002f }, { glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)) })
{

}

void Headlamp::SetHeadlightState(const bool isActive)
{
	headlight.SetIsCameraFlashLightFUniform(isActive);
}

void Headlamp::UpdateHeadlight(const Camera& camera)
{
	if (headlightStartTime > 0.0)
	{
		headlight.SetLightPositionFUniform(camera.GetPosition());
		headlight.SetLightDirectionFUniform(camera.GetForward());
	}
}

void Headlamp::UpdateHeadlightState(Controller& cameraController, int32_t action)
{
	const bool isReleaseActionRegistered = Application::GetInstance().GetTime() - headlightStartTime > cameraController.timeBeforeReleaseRegistered;

	if (action == GLFW_PRESS && headlightStartTime == 0.0)
	{
		SetHeadlightState(true);
		headlightStartTime = Application::GetInstance().GetTime();
	}

	if (action == GLFW_RELEASE && isReleaseActionRegistered)
	{
		SetHeadlightState(false);
		headlightStartTime = 0.0;
	}
}