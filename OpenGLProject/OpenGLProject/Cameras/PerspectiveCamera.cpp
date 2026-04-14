#include "PerspectiveCamera.h"

#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>



PerspectiveCamera::PerspectiveCamera(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inFovY, const float inFarPlane) : Camera(inPosition, inRotation, inFovY, inFarPlane)
{
	UpdateCameraVectors();
}

glm::mat4 PerspectiveCamera::ComputeProjection(const float windowAspectRatio) const
{
	return glm::perspective(glm::radians(fovY), windowAspectRatio, 0.1f, farPlane);
}

glm::mat4 PerspectiveCamera::ComputeView() const
{
	return glm::lookAt(position, position + forward, up);
}

void PerspectiveCamera::UpdateForwardPosition(const float distance)
{
	position += distance * forward;
}

void PerspectiveCamera::UpdateUpPosition(const float distance)
{
	position += distance * up;
}

void PerspectiveCamera::UpdateRightPosition(const float distance)
{
	position += distance * right;
}

void PerspectiveCamera::UpdateRotation(const glm::vec2& offset)
{
	yaw += offset.x;

	// Avoid screen getting flipped when looking more than 90 degrees up or down, by bounding pitch value
	constexpr float maxPitchBeforeFlip = 89.0f;
	pitch = glm::clamp(pitch + offset.y, -maxPitchBeforeFlip, maxPitchBeforeFlip);

	UpdateCameraVectors();
}

void PerspectiveCamera::UpdateCameraVectors()
{
	const float yawInRadians = glm::radians(yaw);
	const float pitchInRadians = glm::radians(pitch);

	// Normalise vectors because their length gets closer to 0 the more you look up or down, which results in slower movement
	glm::vec3 newForward;
	newForward.x = glm::cos(yawInRadians) * glm::cos(pitchInRadians);
	newForward.y = glm::sin(pitchInRadians);
	newForward.z = glm::sin(yawInRadians) * glm::cos(pitchInRadians);
	forward = glm::normalize(newForward);

	right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

	up = glm::normalize(glm::cross(right, forward));
}