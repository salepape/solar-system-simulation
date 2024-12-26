#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

class UniformBuffer;



// Base class that contain all getters/setters needed for all types of Cameras
class Camera
{
public:
	Camera(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inFovY, const float inFarPlane);

	const glm::vec3& GetPosition() const { return position; }
	const glm::vec3& GetUp() const { return up; }
	const glm::vec3& GetForward() const { return forward; }

	void ResetTransform();

	virtual void UpdateForwardPosition(const float distance) = 0;
	virtual void UpdateUpPosition(const float distance) = 0;
	virtual void UpdateRightPosition(const float distance) = 0;
	virtual void UpdateRotation(const glm::vec2& offset) = 0;

	void SetFovY(const float zoomLeft) { fovY = zoomLeft; }

	virtual glm::mat4 ComputeProjection(const float windowAspectRatio) const = 0;
	virtual glm::mat4 ComputeView() const = 0;
	virtual glm::mat4 ComputeInfiniteView() const;

	void SetProjectionViewVUniform(const float windowAspectRatio);
	void SetInfiniteProjectionViewVUniform(const float windowAspectRatio);
	void SetPositionFUniform();

protected:
	glm::vec3 initialPosition{ 0.0f };
	glm::vec3 initialRotation{ 0.0f };

	glm::vec3 position{ 0.0f };

	// Rotation angle around the Forward vector [in degrees]
	float roll{ 0.0f };

	// Rotation angle around the Right vector [in degrees]
	float pitch{ 0.0f };

	// Rotation angle around the Up vector [in degrees]
	float yaw{ 0.0f };

	// Field of view along the y-axis [in degrees]
	float fovY{ 0.0f };
	float farPlane{ 0.0f };

	glm::vec3 up{ 1.0f, 0.0f, 0.0f };
	glm::vec3 right{ 0.0f, 1.0f, 0.0f };
	glm::vec3 forward{ 0.0f, 0.0f, -1.0f };

	UniformBuffer& projectionViewUBO;
	UniformBuffer& positionUBO;

	// Compute new Forward, Right and Up vectors from new Euler Angles
	virtual void UpdateCameraVectors() = 0;
};



#endif // CAMERA_H