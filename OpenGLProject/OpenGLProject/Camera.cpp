#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>


Camera::Camera(const glm::vec3 inPosition) :
	position(inPosition), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)), forward(glm::vec3(0.0f, 0.0f, -1.0f))
{
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

void Camera::ProcessMouseMovement(float xOffset, float yOffset, const bool constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	// Make sure that when Pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		pitch = std::fmin(pitch, 89.0f);
		pitch = std::fmax(pitch, -89.0f);
	}

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(const float yOffset)
{
	if (zoom >= 1.0f && zoom <= 45.0f)
	{
		zoom -= yOffset;
	}

	zoom = std::fmax(zoom, -1.0f);
	zoom = std::fmin(zoom, 45.0f);
}

void Camera::UpdateCameraVectors()
{
	// Normalise vectors because their length gets closer to 0 the more you look up or down, which results in slower movement
	glm::vec3 newForward;
	newForward.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	newForward.y = glm::sin(glm::radians(pitch));
	newForward.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
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

	travelSpeed /= factor;
}