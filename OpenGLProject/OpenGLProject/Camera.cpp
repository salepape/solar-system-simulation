#include "Camera.h"

#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>



Camera::Camera(const glm::vec3& inPosition) : position(inPosition), forward({ 0.0f, 0.0f, -1.0f })
{
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + forward, up);
}

void Camera::UpdateForwardPosition(const float distance)
{
	position += distance * forward;
}

void Camera::UpdateUpPosition(const float distance)
{
	position += distance * up;
}

void Camera::UpdateRightPosition(const float distance)
{
	position += distance * right;
}

void Camera::UpdateRotation(const float xOffset, const float yOffset)
{
	yaw += xOffset;

	// Avoid screen getting flipped by bounding pitch value
	constexpr float maxPitchBeforeFlip = 89.0f;
	pitch = glm::clamp(pitch + yOffset, -maxPitchBeforeFlip, maxPitchBeforeFlip);

	UpdateCameraVectors();
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

	right = glm::normalize(glm::cross(forward, { 0.0f, 1.0f, 0.0f }));

	up = glm::normalize(glm::cross(right, forward));
}