#include "Camera.h"



Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	Forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;

	UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
	Forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;

	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Forward, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
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

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
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

void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	// Normalized vectors, because their length gets closer to 0 the more you look up or down which results in slower movement

	glm::vec3 newForward;
	newForward.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
	newForward.y = glm::sin(glm::radians(Pitch));
	newForward.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
	Forward = glm::normalize(newForward);

	Right = glm::normalize(glm::cross(Forward, WorldUp));

	Up = glm::normalize(glm::cross(Right, Forward));
}


