#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>

#include "ResourceLoader.h"
#include "UniformBuffer.h"
#include "Utils.h"



Camera::Camera(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inFovY, const float inFarPlane) :
	initialPosition(inPosition), initialRotation(inRotation), position(inPosition), roll(inRotation.x), pitch(inRotation.y), yaw(inRotation.z), fovY(inFovY), farPlane(inFarPlane),
	projectionViewUBO(ResourceLoader::GetUBO("ubo_ProjectionView")), positionUBO(ResourceLoader::GetUBO("ubo_CameraPosition"))
{

}

void Camera::ResetTransform()
{
	position = initialPosition;

	roll = initialRotation.x;
	pitch = initialRotation.y;
	yaw = initialRotation.z;

	UpdateCameraVectors();
}

glm::mat4 Camera::ComputeInfiniteView() const
{
	return glm::mat4(glm::mat3(ComputeView()));
}

void Camera::SetProjectionViewVUniform(const float windowAspectRatio)
{
	const glm::mat4& projectionView = ComputeProjection(windowAspectRatio) * ComputeView();
	projectionViewUBO.SetSubData(static_cast<const void*>(glm::value_ptr(projectionView)), Utils::mat4v4Size);
}

void Camera::SetInfiniteProjectionViewVUniform(const float windowAspectRatio)
{
	const glm::mat4& infiniteProjectionView = ComputeProjection(windowAspectRatio) * ComputeInfiniteView();
	projectionViewUBO.SetSubData(static_cast<const void*>(glm::value_ptr(infiniteProjectionView)), Utils::mat4v4Size);
}

void Camera::SetPositionFUniform()
{
	const glm::vec4& position = glm::vec4(GetPosition(), 0.0f);
	positionUBO.SetSubData(static_cast<const void*>(glm::value_ptr(position)), Utils::vec4Size);
}