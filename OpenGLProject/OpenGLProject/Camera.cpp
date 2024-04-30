#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/common.hpp>
#include <iostream>



Camera::Camera(const glm::vec3 inPosition, const float inZoomMaxLevel) :
	position(inPosition), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)), forward(glm::vec3(0.0f, 0.0f, -1.0f)), zoomMaxLevel(inZoomMaxLevel)
{
	zoomLeft = inZoomMaxLevel;

	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + forward, up);
}

void Camera::ProcessKeyboard(const CameraMovement& direction, const float deltaTime)
{
	const float distance = travelSpeed * deltaTime;

	switch (direction)
	{
	case FORWARD:
	{
		position += forward * distance;
		break;
	}
	case BACKWARD:
	{
		position -= forward * distance;
		break;
	}
	case RIGHT:
	{
		position += right * distance;
		break;
	}
	case LEFT:
	{
		position -= right * distance;
		break;
	}
	case UP:
	{
		position += up * distance;
		break;
	}
	case DOWN:
	{
		position -= up * distance;
		break;
	}
	}
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;

	// Avoid screen getting flipped by bounding pitch value
	constexpr float maxPitchBeforeFlip = 89.0f;
	pitch = glm::clamp(pitch + yOffset, -maxPitchBeforeFlip, maxPitchBeforeFlip);

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(const float yOffset)
{
	constexpr float zoomMinLevel = 1.0f;

	if (zoomLeft >= zoomMinLevel && zoomLeft <= zoomMaxLevel)
	{
		zoomLeft -= yOffset;
	}

	zoomLeft = glm::clamp(zoomLeft, zoomMinLevel, zoomMaxLevel);
}

void Camera::UpdateCameraVectors()
{
	const float YawInRadians = glm::radians(yaw);
	const float PitchInRadians = glm::radians(pitch);

	// Normalise vectors because their length gets closer to 0 the more you look up or down, which results in slower movement
	glm::vec3 newForward;
	newForward.x = glm::cos(YawInRadians) * glm::cos(PitchInRadians);
	newForward.y = glm::sin(PitchInRadians);
	newForward.z = glm::sin(YawInRadians) * glm::cos(PitchInRadians);
	forward = glm::normalize(newForward);

	right = glm::normalize(glm::cross(forward, worldUp));

	up = glm::normalize(glm::cross(right, forward));
}

void Camera::IncreaseSpeed(const float factor)
{
	if (travelSpeed > factor * travelSpeedDefault)
	{
		return;
	}

	travelSpeed *= factor;
}

void Camera::DecreaseSpeed(const float factor)
{
	if (travelSpeed < travelSpeedDefault)
	{
		return;
	}

	travelSpeed *= 1.0f / factor;
}