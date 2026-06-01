#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>

#include "Utils/Constants.h"



Transform::Transform(const float inValue) : model(inValue)
{

}

Transform::Transform(const glm::mat4& inModel) : model(inModel)
{

}

Transform::Transform(const glm::vec3& inPosition, const EulerAngles& inRotation)
{
	Translate(inPosition);

	Rotate(inRotation.rollInRad, WorldSpace::XUnitVector);
	Rotate(inRotation.yawInRad, WorldSpace::YUnitVector);
	Rotate(inRotation.pitchInRad, -WorldSpace::ZUnitVector);
}

void Transform::Reset()
{
	model = glm::mat4(1.0f);
}

void Transform::Translate(const glm::vec3& newPosition)
{
	model = glm::translate(model, newPosition);
}

void Transform::Rotate(const float angleInRad, const glm::vec3& rotationVector)
{
	model = glm::rotate(model, angleInRad, rotationVector);
}

void Transform::Scale(const glm::vec3& newScale)
{
	model = glm::scale(model, glm::vec3(newScale));
}
