#include "Transform.h"

#include <glm/common.hpp>					// glm::clamp()
#include <glm/ext/matrix_transform.hpp>		// glm::translate() glm::rotate() glm::scale()
#include <glm/geometric.hpp>				// glm::normalize() glm::cross()
#include <glm/trigonometric.hpp>			// glm::radians()

#include <math.h>



Transform::Transform(const glm::vec3& position, const EulerAngles& rotation)
{
	SetPosition(position);
	SetRotation(rotation);
}

glm::vec3 Transform::GetPosition() const
{
	return glm::vec3(model[3]);
}

void Transform::SetPosition(const glm::vec3& inPosition)
{
	model[3] = glm::vec4(inPosition, 1.0f);
}

void Transform::SetRotation(const RotationMatrix& inRotation)
{
	UpdateModelMatrix(inRotation);

	// Deduce Euler angles from Model matrix, as user defined its own LookAt vectors
	rotation.rollInRad = std::atan2<float>(model[2][3], model[3][3]);
	rotation.yawInRad = std::atan2<float>(-model[1][3], std::sqrt(static_cast<float>(std::pow(model[2][3], 2) + std::pow(model[3][3], 2))));
	rotation.pitchInRad = std::atan2<float>(model[1][2], model[1][1]);
}

void Transform::UpdateModelMatrix(std::optional<std::reference_wrapper<const RotationMatrix>> inRotation)
{
	const bool isRotationProvided = inRotation.has_value();

	model[0] = glm::vec4(isRotationProvided ? inRotation.value().get().GetRightVector() : GetRightVector(), 0.0f);
	model[1] = glm::vec4(isRotationProvided ? inRotation.value().get().GetUpVector() : GetUpVector(), 0.0f);
	model[2] = glm::vec4(isRotationProvided ? inRotation.value().get().GetForwardVector() : GetForwardVector(), 0.0f);
}

void Transform::Reset()
{
	model = glm::mat4(1.0f);
}

void Transform::Translate(const glm::vec3& newPosition)
{
	model = glm::translate(model, newPosition);
}

void Transform::Rotate(const float angleInRad, const glm::vec3& rotationAxis)
{
	model = glm::rotate(model, angleInRad, rotationAxis);
}

void Transform::Scale(const glm::vec3& newScale)
{
	model = glm::scale(model, glm::vec3(newScale));
}

void Transform::SetRotation(const EulerAngles& inRotationInRad)
{
	rotation = inRotationInRad;

	UpdateLookAtVectors();
}

void Transform::UpdateRotation(const EulerAngles& newAngles)
{
	rotation.yawInRad += newAngles.yawInRad;

	// Avoid screen getting flipped when looking more than 90 degrees up or down, by bounding pitch value
	const float maxPitchBeforeFlip = glm::radians(89.0f);
	rotation.pitchInRad = glm::clamp(rotation.pitchInRad + newAngles.pitchInRad, -maxPitchBeforeFlip, maxPitchBeforeFlip);

	UpdateLookAtVectors();
}

void Transform::UpdateLookAtVectors()
{
	// Normalise vectors because their length gets closer to 0 the more we look up/down, which results in slower movement
	glm::vec3 newForwardVector{ 0.0f };
	newForwardVector.x = glm::cos(rotation.yawInRad) * glm::cos(rotation.pitchInRad);
	newForwardVector.y = glm::sin(rotation.pitchInRad);
	newForwardVector.z = glm::sin(rotation.yawInRad) * glm::cos(rotation.pitchInRad);

	forwardVector = glm::normalize(newForwardVector);
	rightVector = glm::normalize(glm::cross(forwardVector, WorldSpace::YUnitVector));
	upVector = glm::normalize(glm::cross(rightVector, forwardVector));

	UpdateModelMatrix();
}
