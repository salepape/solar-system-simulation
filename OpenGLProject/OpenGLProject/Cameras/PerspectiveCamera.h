#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "Camera.h"

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>



// Camera computing	a perspective projection-view matrix each frame from the update loop
class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inFovY, const float inFarPlane);

	void UpdateForwardPosition(const float distance) override;
	void UpdateUpPosition(const float distance) override;
	void UpdateRightPosition(const float distance) override;

	// Update pitch and yaw values only, and not roll
	void UpdateRotation(const glm::vec2& offset) override;

	glm::mat4 ComputeProjection(const float windowAspectRatio) const override;
	glm::mat4 ComputeView() const override;

protected:
	// Compute new Forward, Right and Up vectors from new Euler Angles
	void UpdateCameraVectors() override;
};



#endif // PERSPECTIVE_CAMERA_H