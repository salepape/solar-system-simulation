#include "Headlamp.h"

#include <glfw/glfw3.h>
#include <glm/trigonometric.hpp>

#include "Application/Application.h"
#include "Application/ApplicationControls.h"
#include "Cameras/Camera.h"



Headlamp::Headlamp(const glm::vec3& inPosition) :
	headlight(inPosition, glm::vec3(0.0f, 0.0f, -1.0f), ReflectionParams{ glm::vec3(0.2f), glm::vec3(0.8f), glm::vec3(1.0f) }, AttenuationParams{ 1.0f, 0.007f, 0.0002f }, SpotParams{ glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)) })
{

}

void Headlamp::SetHeadlightState(const bool isActive)
{
	headlight.SetIsCameraFlashLightFUniform(isActive);
}

void Headlamp::UpdateHeadlight(const Camera& camera)
{
	if (headlightStartTime > 0.0f)
	{
		headlight.SetLightPositionFUniform(camera.GetPosition());
		headlight.SetLightDirectionFUniform(camera.GetForward());
	}
}

void Headlamp::UpdateHeadlightState(const int32_t action)
{
	const bool isReleaseActionRegistered = Application::GetInstance().GetElapsedTime() - headlightStartTime > ApplicationControls::KEY_RELEASE_SENSITIVITY;

	if (action == GLFW_PRESS && headlightStartTime == 0.0f)
	{
		SetHeadlightState(true);
		headlightStartTime = Application::GetInstance().GetElapsedTime();
	}

	if (action == GLFW_RELEASE && isReleaseActionRegistered)
	{
		SetHeadlightState(false);
		headlightStartTime = 0.0;
	}
}
