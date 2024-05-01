#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "Shader.h"

class UniformBuffer;



// Fly-like camera class
class Camera
{
public:
	Camera(const glm::vec3& inPosition, const float fovY, const std::vector<unsigned int>& shaderIDs);

	const glm::vec3& GetPosition() const { return position; }
	const glm::vec3& GetUp() const { return up; }

	void UpdateForwardPosition(const float distance);
	void UpdateUpPosition(const float distance);
	void UpdateRightPosition(const float distance);
	void UpdateRotation(const glm::vec2& offset);

	void SetFovY(const float zoomLeft) { fovY = zoomLeft; }

	glm::mat4 ComputeProjectionMatrix(const float windowAspectRatio) const;
	glm::mat4 ComputeViewMatrix() const;

	void SetProjectionViewUniform(const float windowAspectRatio);
	void SetInfiniteProjectionViewUniform(const float windowAspectRatio);
	void SetPositionUniform(Shader& shader);

private:
	glm::vec3 position;

	// Field of view along the y-axis [in degrees]
	float fovY{ 0.0f };

	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 forward;

	// [in degrees]
	float yaw{ -90.0f };

	// [in degrees]
	float pitch{ 0.0f };

	UniformBuffer* ubo{ nullptr };

	// Compute new Forward, Right and Up vectors from new Euler Angles
	void UpdateCameraVectors();

	void AllocateProjectionView(const std::vector<unsigned int>& shaderIDs);
};



#endif // CAMERA_H