#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Buffers/UniformBuffer.h"
#include "Scene/Transform.h"



// View matrix type to use when computing the Projection-View matrix for a given Scene Entity and frame 
enum class ViewMode
{
	FiniteLookAt = 0,
	InfiniteLookAt,
};

// Base class that contain all getters/setters needed for all types of Cameras
class Camera
{
public:
	Camera(const glm::vec3& inPosition, const EulerAngles& inRotation, const float inFovY, const float inFarPlane);

	glm::vec3 GetPosition() const { return transform.GetPosition(); }
	const glm::vec3& GetCameraUp() const { return cameraUp; }
	[[maybe_unused]] const glm::vec3& GetCameraRight() const { return cameraRight; }
	const glm::vec3& GetCameraForward() const { return cameraForward; }

	void SetInitialTransform(const glm::vec3& inPosition, const EulerAngles& inRotation);
	void SetTransform(const glm::vec3& inPosition, const EulerAngles& inRotation);
	void ResetTransform();

	virtual void Translate(const float deltaPosition, const glm::vec3& direction) = 0;
	virtual void UpdateRotation(const EulerAngles& deltaRotation) = 0;

	void SetFovY(const float zoomLeft) { fovY = zoomLeft; }

	virtual glm::mat4 ComputeProjection(const float windowAspectRatio) const = 0;
	virtual glm::mat4 ComputeView() const = 0;
	virtual glm::mat4 ComputeInfiniteView() const;

	void SetProjectionViewVUniform(const ViewMode viewMode, const float windowAspectRatio) const;
	void SetPositionFUniform() const;

protected:
	EulerAngles initialRotation{ 0.0f };
	EulerAngles rotation;
	Transform transformAtStart;
	Transform transform;

	// Field of view along the y-axis [in degrees]
	float fovY{ 0.0f };
	float farPlane{ 0.0f };

	UniformBuffer vuboProjectionView;
	UniformBuffer fuboCameraPosition;

	glm::vec3 cameraUp{ 1.0f, 0.0f, 0.0f };
	glm::vec3 cameraRight{ 0.0f, 1.0f, 0.0f };
	glm::vec3 cameraForward{ 0.0f, 0.0f, -1.0f };

	// Compute new Camera Forward, Right and Up vectors from new Euler Angles
	virtual void UpdateCameraVectors() = 0;
};



#endif // CAMERA_H
