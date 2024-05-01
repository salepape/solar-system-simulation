#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>



// Fly-like camera class
class Camera
{
public:
	Camera(const glm::vec3& inPosition);

	const glm::vec3& GetPosition() const { return position; }
	const glm::vec3& GetUp() const { return up; }
	glm::mat4 GetViewMatrix() const;

	void UpdateForwardPosition(const float distance);
	void UpdateUpPosition(const float distance);
	void UpdateRightPosition(const float distance);
	void UpdateRotation(const glm::vec2& offset);

private:
	glm::vec3 position;

	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 forward;

	// [in degrees]
	float yaw{ -90.0f };

	// [in degrees]
	float pitch{ 0.0f };

	// Compute new Forward, Right and Up vectors from new Euler Angles
	void UpdateCameraVectors();
};



#endif // CAMERA_H