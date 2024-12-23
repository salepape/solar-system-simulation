#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

class Shader;
class UniformBuffer;



class Camera
{
public:
	Camera(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inFovY, const float inFarPlane);

	const glm::vec3& GetPosition() const { return position; }
	const glm::vec3& GetUp() const { return up; }
	const glm::vec3& GetForward() const { return forward; }

	void ResetTransform();

	void UpdateForwardPosition(const float distance);
	void UpdateUpPosition(const float distance);
	void UpdateRightPosition(const float distance);
	void UpdateRotation(const glm::vec2& offset);

	void SetFovY(const float zoomLeft) { fovY = zoomLeft; }

	glm::mat4 ComputePerspectiveProjection(const float windowAspectRatio) const;
	glm::mat4 ComputeView() const;
	glm::mat4 ComputeInfiniteView() const;

	void SetProjectionViewVUniform(const float windowAspectRatio);
	void SetInfiniteProjectionViewVUniform(const float windowAspectRatio);
	void SetPositionFUniform();

private:
	glm::vec3 initialPosition{ 0.0f };
	glm::vec3 initialRotation{ 0.0f };

	glm::vec3 position{ 0.0f };

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
	void UpdateCameraVectors();
};



#endif // CAMERA_H