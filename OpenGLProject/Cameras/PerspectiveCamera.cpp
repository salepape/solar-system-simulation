#include "PerspectiveCamera.h"

#include <glm/ext/matrix_clip_space.hpp>	// glm::perspective()
#include <glm/ext/matrix_transform.hpp>		// glm::lookAt()
#include <glm/trigonometric.hpp>			// glm::radians()



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
	return glm::lookAt(cameraPosition, cameraPosition + transform.GetForwardVector(), transform.GetUpVector());
}

void PerspectiveCamera::Translate(const float deltaPosition, const glm::vec3& direction)
{
	transform.Translate(deltaPosition * direction);
}

void PerspectiveCamera::Rotate(const EulerAngles& deltaRotation)
{
	transform.UpdateRotation(deltaRotation);
}
