#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtc/matrix_transform.hpp>		// for cos, sin and lookAt functions



// Defines several possible options for camera movement (used as abstraction to stay away from window-system specific input methods)
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default Camera values
const float YAW = -90.0f;				// [in degrees]	
const float PITCH = 0.0f;				// [in degrees]
const float SPEED = 20.0f;				// [in km/s]
const float SENSITIVITY = 0.1f;			// [dimensionless]
const float ZOOM = 45.0f;				// [in degrees]



// Camera class that processes input and calculates the corresponding Euler angles, vectors and matrices for OpenGL to use
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// Compute View matrix using LookAt matrix
	glm::mat4 GetViewMatrix();

	// Process input received from any keyboard-like input system (requires enum camera movement option)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// Process input received from a mouse input system (requires offset value in both x and y direction)
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

	// Process input received from a mouse scroll-wheel event (only requires input on the vertical wheel-axis)
	void ProcessMouseScroll(float yoffset);



private:
	// Compute updated Forward, Right and Up vectors from the Camera's updated Euler Angles
	void UpdateCameraVectors();
};

#endif

