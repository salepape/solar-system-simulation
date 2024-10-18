#include "Camera.h"

#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

#include "ResourceLoader.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "Utils.h"



Camera::Camera(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inFovY, const float inFarPlane) :
	initialPosition(inPosition), initialRotation(inRotation), position(inPosition), pitch(inRotation.y), yaw(inRotation.z), fovY(inFovY), farPlane(inFarPlane),
	projectionViewUBO(ResourceLoader::GetUBO("ubo_ProjectionView")), positionUBO(ResourceLoader::GetUBO("ubo_CameraPosition"))
{
	UpdateCameraVectors();
}

void Camera::ResetTransform()
{
	position = initialPosition;

	pitch = initialRotation.y;
	yaw = initialRotation.z;

	UpdateCameraVectors();
}

glm::mat4 Camera::ComputePerspectiveProjection(const float windowAspectRatio) const
{
	return glm::perspective(glm::radians(fovY), windowAspectRatio, 0.1f, farPlane);
}

glm::mat4 Camera::ComputeView() const
{
	return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::ComputeInfiniteView() const
{
	return glm::mat4(glm::mat3(ComputeView()));
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

void Camera::UpdateRotation(const glm::vec2& offset)
{
	yaw += offset.x;

	// Avoid screen getting flipped when looking more than 90 degrees up or down, by bounding pitch value
	constexpr float maxPitchBeforeFlip = 89.0f;
	pitch = glm::clamp(pitch + offset.y, -maxPitchBeforeFlip, maxPitchBeforeFlip);

	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
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

void Camera::SetProjectionViewVUniform(const float windowAspectRatio)
{
	const glm::mat4& projectionView = ComputePerspectiveProjection(windowAspectRatio) * ComputeView();
	projectionViewUBO.SetSubData(static_cast<const void*>(glm::value_ptr(projectionView)), Utils::mat4v4Size);
}

void Camera::SetInfiniteProjectionViewVUniform(const float windowAspectRatio)
{
	const glm::mat4& infiniteProjectionView = ComputePerspectiveProjection(windowAspectRatio) * ComputeInfiniteView();
	projectionViewUBO.SetSubData(static_cast<const void*>(glm::value_ptr(infiniteProjectionView)), Utils::mat4v4Size);
}

void Camera::SetPositionFUniform()
{
	const glm::vec4& position = glm::vec4(GetPosition(), 0.0f);
	positionUBO.SetSubData(static_cast<const void*>(glm::value_ptr(position)), Utils::vec4Size);
}