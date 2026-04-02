#include "Camera.h"

#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"



Camera::Camera(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inFovY, const float inFarPlane) :
	initialPosition(inPosition), initialRotation(inRotation), position(inPosition), roll(inRotation.x), pitch(inRotation.y), yaw(inRotation.z), fovY(inFovY), farPlane(inFarPlane),
	vuboProjectionView("vubo_ProjectionView", UniformShaderGroup::PROJECTION_VIEW), fuboCameraPosition("fubo_CameraPosition", UniformShaderGroup::LINE_OF_SIGHT)
{
	// Stored in a GLSL struct (containing a single Uniform field) with in Fragment Shader
	UniformGLSLStruct vuboProjectionViewStruct;
	vuboProjectionViewStruct.AddUniformField(static_cast<const void*>(glm::value_ptr(glm::mat4(0.0f))), GLSLConstants::mat4v4SizeInBytes);
	vuboProjectionView.SetData(vuboProjectionViewStruct);

	// Not stored in a GLSL struct in Fragment Shader
	fuboCameraPosition.SetData(static_cast<const void*>(glm::value_ptr(glm::vec4(0.0f))), GLSLConstants::vec4SizeInBytes);
}

void Camera::SetInitialTransform(const glm::vec3& inPosition, const glm::vec3& inRotation)
{
	SetTransform(inPosition, inRotation);

	initialPosition = inPosition;
	initialRotation = inRotation;
}

void Camera::SetTransform(const glm::vec3& inPosition, const glm::vec3& inRotation)
{
	position = inPosition;

	roll = inRotation.x;
	pitch = inRotation.y;
	yaw = inRotation.z;

	UpdateCameraVectors();
}

void Camera::ResetTransform()
{
	SetTransform(initialPosition, glm::vec3(initialRotation.x, initialRotation.y, initialRotation.z));
}

glm::mat4 Camera::ComputeInfiniteView() const
{
	return glm::mat4(glm::mat3(ComputeView()));
}

void Camera::SetProjectionViewVUniform(const float windowAspectRatio) const
{
	const glm::mat4& projectionView = ComputeProjection(windowAspectRatio) * ComputeView();
	vuboProjectionView.SetSubData(static_cast<const void*>(glm::value_ptr(projectionView)), GLSLConstants::mat4v4SizeInBytes, 0);
}

void Camera::SetInfiniteProjectionViewVUniform(const float windowAspectRatio) const
{
	const glm::mat4& infiniteProjectionView = ComputeProjection(windowAspectRatio) * ComputeInfiniteView();
	vuboProjectionView.SetSubData(static_cast<const void*>(glm::value_ptr(infiniteProjectionView)), GLSLConstants::mat4v4SizeInBytes, 0);
}

void Camera::SetPositionFUniform() const
{
	const glm::vec4& position = glm::vec4(GetPosition(), 0.0f);
	fuboCameraPosition.SetSubData(static_cast<const void*>(glm::value_ptr(position)), GLSLConstants::vec4SizeInBytes, 0);
}