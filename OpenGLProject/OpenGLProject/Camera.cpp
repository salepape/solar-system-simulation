#include "Camera.h"



Camera::Camera(const glm::vec3 position, const glm::vec3 up, const float yaw, const float pitch) :
	Forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;

	UpdateCameraVectors();
}

Camera::Camera(const float posX, const float posY, const float posZ, const float upX, const float upY, const float upZ, const float yaw, const float pitch) :
	Forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;

	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Forward, Up);
}

void Camera::ProcessKeyboard(const Camera_Movement direction, const float deltaTime)
{
	const float distance = MovementSpeed * deltaTime;
	switch (direction)
	{
	case FORWARD:
		Position += Forward * distance;
		break;
	case BACKWARD:
		Position -= Forward * distance;
		break;
	case RIGHT:
		Position += Right * distance;
		break;
	case LEFT:
		Position -= Right * distance;
		break;
	case UP:
		Position += Up * distance;
		break;
	case DOWN:
		Position -= Up * distance;
		break;
	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, const bool constrainPitch)
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

void Camera::ProcessMouseScroll(const float yoffset)
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
	// Normalise vectors because their length gets closer to 0 the more you look up or down, which results in slower movement
	glm::vec3 newForward;
	newForward.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
	newForward.y = glm::sin(glm::radians(Pitch));
	newForward.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
	Forward = glm::normalize(newForward);

	Right = glm::normalize(glm::cross(Forward, WorldUp));

	Up = glm::normalize(glm::cross(Right, Forward));
}


