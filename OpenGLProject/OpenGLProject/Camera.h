#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>



// Defines several options for camera movement (used as abstraction to stay away from window-system specific input methods)
enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

// Camera class that processes input and calculates its Euler angles, vectors and matrices
class Camera
{
public:
	Camera(const glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	~Camera();

	// Process input received from any keyboard-like input system (requires enum camera movement option)
	void ProcessKeyboard(const CameraMovement& direction, const float deltaTime);

	// Process input received from a mouse input system (requires offset value in both x and y direction)
	void ProcessMouseMovement(float xoffset, float yoffset, const bool constrainPitch = true);

	// Process input received from a mouse scroll-wheel event (only requires input on the vertical wheel-axis)
	void ProcessMouseScroll(const float yoffset);

	void IncreaseSpeed(const float factor);
	void DecreaseSpeed(const float factor);

	const glm::vec3& GetPosition() const { return position; }
	const glm::vec3& GetUp() const { return up; }

	float GetZoom() const { return zoom; }

	glm::mat4 GetViewMatrix() const;

private:
	glm::vec3 position;
	glm::vec3 worldUp;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 forward;

	// [in degrees]
	float yaw{ -90.0f };

	// [in degrees]
	float pitch{ 0.0f };

	// [in km/s]
	float travelSpeed{ 20.0f };
	float travelSpeedDefault{ 20.0f };

	// [dimensionless]
	float mouseSensitivity{ 0.1f };

	// [in degrees]
	float zoom{ 45.0f };

	// Compute updated Forward, Right and Up vectors from the Camera's updated Euler Angles
	void UpdateCameraVectors();
};



#endif // CAMERA_H