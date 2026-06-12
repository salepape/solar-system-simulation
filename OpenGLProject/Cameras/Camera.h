#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
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

	const Transform& GetTransform() const { return transform; }
	void SetInitialTransform(const glm::vec3& inPosition, const EulerAngles& inRotation);
	void SetTransform(const glm::vec3& inPosition, const EulerAngles& inRotation);
	void SetTransformToStart();

	// @todo - Check if position interpolation (movement) managed by glm::translate()
	// Update position of camera by delta distance
	virtual void Translate(const float deltaPosition, const glm::vec3& direction) = 0;

	// Update rotation by new yaw/pitch Euler Angles around new Up/Right camera vectors
	// (no rotation by roll around Forward camera vector, as we do not want rotation around normal to screen)
	virtual void Rotate(const EulerAngles& deltaRotation) = 0;

	void SetFovY(const float zoomLeft) { fovY = zoomLeft; }

	virtual glm::mat4 ComputeProjection(const float windowAspectRatio) const = 0;
	virtual glm::mat4 ComputeView() const = 0;
	virtual glm::mat4 ComputeInfiniteView() const;

	void SetProjectionViewVUniform(const ViewMode viewMode, const float windowAspectRatio) const;
	void SetPositionFUniform() const;

protected:
	Transform transformAtStart;
	Transform transform;

	// Field of view along the y-axis [in degrees]
	float fovY{ 0.0f };
	float farPlane{ 0.0f };

	UniformBuffer vuboProjectionView;
	UniformBuffer fuboCameraPosition;
};



#endif // CAMERA_H
