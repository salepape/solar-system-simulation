#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "Camera.h"

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>



// Scene camera computing a perspective projection-view matrix each frame from the update loop
class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(const glm::vec3& inPosition, const EulerAngles& inRotation, const float inFovY, const float inFarPlane);

	void UpdateCameraForwardPosition(const float deltaDistance) override;
	void UpdateCameraUpPosition(const float deltaDistance) override;
	void UpdateCameraRightPosition(const float deltaDistance) override;

	// Update raw and pitch values only, not roll (rotation around normal to screen)
	void UpdateRotation(const EulerAngles& deltaRotation) override;

	glm::mat4 ComputeProjection(const float windowAspectRatio) const override;
	glm::mat4 ComputeView() const override;

protected:
	// Compute new Forward, Right and Up vectors from new Euler Angles
	void UpdateCameraVectors() override;
};



#endif // PERSPECTIVE_CAMERA_H
