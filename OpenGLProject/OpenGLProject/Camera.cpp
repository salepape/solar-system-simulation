#include "Camera.h"

#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/trigonometric.hpp>

#include "UniformBuffer.h"
#include "Utils.h"



Camera::Camera(const glm::vec3& inPosition, const float inFovY, const std::vector<unsigned int>& shaderIDs) :
	position(inPosition), fovY(inFovY), forward({ 0.0f, 0.0f, -1.0f })
{
	UpdateCameraVectors();
	AllocateProjectionView(shaderIDs);
}

glm::mat4 Camera::ComputeProjectionMatrix(const float windowAspectRatio) const
{
	return glm::perspective(glm::radians(fovY), windowAspectRatio, 0.1f, 1000.0f);
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

	right = glm::normalize(glm::cross(forward, { 0.0f, 1.0f, 0.0f }));

	up = glm::normalize(glm::cross(right, forward));
}

void Camera::AllocateProjectionView(const std::vector<unsigned int>& shaderIDs)
{
	ubo = new UniformBuffer(shaderIDs, "ubo_ProjectionView", static_cast<size_t>(Utils::mat4v4Size));
}

void Camera::SetProjectionViewUniform(const float windowAspectRatio)
{
	ubo->InitSubData({ { static_cast<const void*>(glm::value_ptr(ComputeProjectionMatrix(windowAspectRatio) * ComputeViewMatrix())), Utils::mat4v4Size } });
}

void Camera::SetInfiniteProjectionViewUniform(const float windowAspectRatio)
{
	ubo->InitSubData({ { static_cast<const void*>(glm::value_ptr(ComputeProjectionMatrix(windowAspectRatio) * glm::mat4(glm::mat3(ComputeViewMatrix())))), Utils::mat4v4Size } });
}

void Camera::SetPositionUniform(Shader& shader)
{
	shader.Enable();
	shader.setUniformVec3("fu_CameraPosition", GetPosition());
	shader.Disable();
}