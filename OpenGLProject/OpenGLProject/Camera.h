#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtc/matrix_transform.hpp>



// Defines several possible options for camera movement (used as abstraction to stay away from window-system specific input methods)
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



// Camera class that processes input and calculates the corresponding Euler angles, vectors and matrices for OpenGL to use
class Camera
{
private:
	// Compute updated Forward, Right and Up vectors from the Camera's updated Euler Angles
	void UpdateCameraVectors();

public:
	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw{ 0.0f };
	float Pitch{ 0.0f };

	float MovementSpeed{ 0.0f };
	float MouseSensitivity{ 0.0f };
	float Zoom{ 0.0f };

	Camera(const glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), const float yaw = YAW, const float pitch = PITCH);
	Camera(const float posX, const float posY, const float posZ, const float upX, const float upY, const float upZ, const float yaw, const float pitch);

	// Compute View matrix using LookAt matrix
	glm::mat4 GetViewMatrix() const;

	// Process input received from any keyboard-like input system (requires enum camera movement option)
	void ProcessKeyboard(const CameraMovement direction, const float deltaTime);

	// Process input received from a mouse input system (requires offset value in both x and y direction)
	void ProcessMouseMovement(float xoffset, float yoffset, const bool constrainPitch = true);

	// Process input received from a mouse scroll-wheel event (only requires input on the vertical wheel-axis)
	void ProcessMouseScroll(const float yoffset);
};



#endif // CAMERA_H

