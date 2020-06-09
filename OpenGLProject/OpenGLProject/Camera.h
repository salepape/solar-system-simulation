#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>		// for glm::lookAt function



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
const float YAW = -90.0f;				// (in degrees)	
const float PITCH = 0.0f;				// (in degrees)
const float SPEED = 20.0f;				// (in "distance unit chosen ~ 100km" / seconds)
const float SENSITIVITY = 0.1f;			// (dimensionless)
const float ZOOM = 45.0f;				// (in degrees)



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
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :
		Forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;

		UpdateCameraVectors();
	}

	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
		Forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;

		UpdateCameraVectors();
	}

	// Compute View matrix using LookAt matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Forward, Up);
	}

	// Process input received from any keyboard-like input system (requires enum camera movement option)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float distance = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Forward * distance;
		if (direction == BACKWARD)
			Position -= Forward * distance;
		if (direction == RIGHT)
			Position += Right * distance;
		if (direction == LEFT)
			Position -= Right * distance;
		if (direction == UP)
			Position += Up * distance;
		if (direction == DOWN)
			Position -= Up * distance;
	}

	// Process input received from a mouse input system (requires offset value in both x and y direction)
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when Pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		UpdateCameraVectors();
	}

	// Process input received from a mouse scroll-wheel event (only requires input on the vertical wheel-axis)
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	// Compute updated Forward, Right and Up vectors from the Camera's updated Euler Angles
	void UpdateCameraVectors()
	{
		// Normalized vectors, because their length gets closer to 0 the more you look up or down which results in slower movement

		glm::vec3 newForward;
		newForward.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		newForward.y = sin(glm::radians(Pitch));
		newForward.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Forward = glm::normalize(newForward);

		Right = glm::normalize(glm::cross(Forward, WorldUp));

		Up = glm::normalize(glm::cross(Right, Forward));
	}
};

#endif

