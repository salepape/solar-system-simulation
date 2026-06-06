#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "Camera.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Scene/Transform.h"



// Scene camera computing a perspective projection-view matrix each frame from the update loop
class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(const glm::vec3& inPosition, const EulerAngles& inRotation, const float inFovY, const float inFarPlane);

	void Translate(const float deltaPosition, const glm::vec3& direction) override;
	void Rotate(const EulerAngles& deltaRotation) override;

	glm::mat4 ComputeProjection(const float windowAspectRatio) const override;
	glm::mat4 ComputeView() const override;
};



#endif // PERSPECTIVE_CAMERA_H
