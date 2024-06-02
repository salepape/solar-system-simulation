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
	initialPosition(position), initialRotation(inRotation), position(inPosition), pitch(inRotation.y), yaw(inRotation.z), fovY(inFovY), farPlane(inFarPlane), projectionViewUBO(ResourceLoader::GetUBO("ubo_ProjectionView")), positionUBO(ResourceLoader::GetUBO("ubo_CameraPosition"))
{
	UpdateCameraVectors();
}

void Camera::Reset()
{
	position = initialPosition;

	pitch = initialRotation.y;
	yaw = initialRotation.z;

	UpdateCameraVectors();
}

glm::mat4 Camera::ComputeProjectionMatrix(const float windowAspectRatio) const
{
	return glm::perspective(glm::radians(fovY), windowAspectRatio, 0.1f, farPlane);
}

glm::mat4 Camera::ComputeViewMatrix() const
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

void Camera::UpdateRotation(const glm::vec2& offset)
{
	yaw += offset.x;

	// Avoid screen getting flipped by bounding pitch value
	constexpr float maxPitchBeforeFlip = 89.0f;
	pitch = glm::clamp(pitch + offset.y, -maxPitchBeforeFlip, maxPitchBeforeFlip);

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

	right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

	up = glm::normalize(glm::cross(right, forward));
}

void Camera::SetProjectionViewVUniform(const float windowAspectRatio)
{
	projectionViewUBO.InitSubData({ { static_cast<const void*>(glm::value_ptr(ComputeProjectionMatrix(windowAspectRatio) * ComputeViewMatrix())), Utils::mat4v4Size } });
}

void Camera::SetInfiniteProjectionViewVUniform(const float windowAspectRatio)
{
	projectionViewUBO.InitSubData({ { static_cast<const void*>(glm::value_ptr(ComputeProjectionMatrix(windowAspectRatio) * glm::mat4(glm::mat3(ComputeViewMatrix())))), Utils::mat4v4Size } });
}

void Camera::SetPositionFUniform()
{
	positionUBO.InitSubData({ { static_cast<const void*>(glm::value_ptr(glm::vec4(GetPosition(), 0.0f))), Utils::vec4Size } });
}