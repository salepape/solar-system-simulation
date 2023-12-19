#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtc/matrix_transform.hpp>



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

// Default Camera values
constexpr float YAW = -90.0f;				// [in degrees]	
constexpr float PITCH = 0.0f;				// [in degrees]
constexpr float SPEED = 20.0f;				// [in km/s]
constexpr float SENSITIVITY = 0.1f;			// [dimensionless]
constexpr float ZOOM = 45.0f;				// [in degrees]



// Camera class that processes input and calculates the corresponding Euler angles, vectors and matrices
class Camera
{
public:
	Camera(const glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), const float yaw = YAW, const float pitch = PITCH);

	glm::vec3 position;
	glm::vec3 worldUp;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 forward;

	float yaw{ 0.0f };
	float pitch{ 0.0f };

	float movementSpeed{ 0.0f };
	float mouseSensitivity{ 0.0f };
	float zoom{ 0.0f };

	// Compute View matrix using LookAt matrix
	glm::mat4 GetViewMatrix() const;

	// Process input received from any keyboard-like input system (requires enum camera movement option)
	void ProcessKeyboard(const CameraMovement direction, const float deltaTime);

	// Process input received from a mouse input system (requires offset value in both x and y direction)
	void ProcessMouseMovement(float xoffset, float yoffset, const bool constrainPitch = true);

	// Process input received from a mouse scroll-wheel event (only requires input on the vertical wheel-axis)
	void ProcessMouseScroll(const float yoffset);

private:
	// Compute updated Forward, Right and Up vectors from the Camera's updated Euler Angles
	void UpdateCameraVectors();
};



#endif // CAMERA_H

