#include "PerspectiveCamera.h"

#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "Scene/Transform.h"



PerspectiveCamera::PerspectiveCamera(const glm::vec3& inPosition, const EulerAngles& inRotation, const float inFovY, const float inFarPlane) :
	Camera(inPosition, inRotation, inFovY, inFarPlane)
{

}

glm::mat4 PerspectiveCamera::ComputeProjection(const float windowAspectRatio) const
{
	return glm::perspective(glm::radians(fovY), windowAspectRatio, 0.1f, farPlane);
}

glm::mat4 PerspectiveCamera::ComputeView() const
{
	const glm::vec3& cameraPosition = transform.GetPosition();
	return glm::lookAt(cameraPosition, cameraPosition + cameraForward, cameraUp);
}

void PerspectiveCamera::Translate(const float deltaPosition, const glm::vec3& direction)
{
	transform.Translate(deltaPosition * direction);
}

void PerspectiveCamera::UpdateRotation(const EulerAngles& deltaRotation)
{
	rotation.yawInRad += deltaRotation.yawInRad;

	// Avoid screen getting flipped when looking more than 90 degrees up or down, by bounding pitch value
	constexpr float maxPitchBeforeFlip = 89.0f;
	rotation.pitchInRad = glm::clamp(rotation.pitchInRad + deltaRotation.pitchInRad, -maxPitchBeforeFlip, maxPitchBeforeFlip);

	UpdateCameraVectors();
}

void PerspectiveCamera::UpdateCameraVectors()
{
	// Normalise vectors because their length gets closer to 0 the more you look up or down, which results in slower movement
	glm::vec3 newCameraForward{ 0.0f };
	newCameraForward.x = glm::cos(rotation.yawInRad) * glm::cos(rotation.pitchInRad);
	newCameraForward.y = glm::sin(rotation.pitchInRad);
	newCameraForward.z = glm::sin(rotation.yawInRad) * glm::cos(rotation.pitchInRad);
	cameraForward = glm::normalize(newCameraForward);

	cameraRight = glm::normalize(glm::cross(cameraForward, WorldSpace::YUnitVector));

	cameraUp = glm::normalize(glm::cross(cameraRight, cameraForward));
}
