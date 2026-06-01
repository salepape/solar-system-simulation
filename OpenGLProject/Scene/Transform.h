#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>



// Unit Vectors in World Space
namespace WorldSpace
{
	static const glm::vec3 XUnitVector(1.0f, 0.0f, 0.0f);
	static const glm::vec3 YUnitVector(0.0f, 1.0f, 0.0f);
	static const glm::vec3 ZUnitVector(0.0f, 0.0f, 1.0f);
};

// Gather classic rotation angles [in Radians] into a single struct
struct EulerAngles
{
	// Rotation angle [in radians] around the World X vector
	float rollInRad{ 0.0f };

	// Rotation angle [in radians] around the World Y vector
	float yawInRad{ 0.0f };

	// Rotation angle [in radians] around the World Z vector
	float pitchInRad{ 0.0f };
};

struct Rotation
{
	//RotationAngles anglesInRad;
	glm::mat3 matrix;

	Rotation(const glm::vec3& firstColumn, const glm::vec3& secondColumn, const glm::vec3& thirdColumn) :
		matrix(firstColumn, secondColumn, thirdColumn) {
	}
};

// Correspond to the Scene Transform (position, rotation, scale) of the Scene Entity
class Transform
{
public:
	Transform() = default;
	Transform(const float inValue);
	Transform(const glm::mat4& inModel);
	[[maybe_unused]] Transform(const glm::vec3& inPosition, const EulerAngles& inRotation);

	~Transform() = default;

	const glm::mat4& Get() const { return model; }
	void Set(const glm::mat4& inModel) { model = inModel; }

	glm::vec3 GetPosition() const
	{
		return glm::vec3(model[3]);
	}

	void SetPosition(const glm::vec3& inPosition)
	{
		model[3] = glm::vec4(inPosition, 1.0f);
	}

	void SetRotation(const Rotation& rotation)
	{
		model[0] = glm::vec4(rotation.matrix[0], 0.0f);
		model[1] = glm::vec4(rotation.matrix[1], 0.0f);
		model[2] = glm::vec4(rotation.matrix[2], 0.0f);
	}

	// @todo - To be moved to Movement Component or similar?
	void Reset();
	void Translate(const glm::vec3& newPosition);
	void Rotate(const float angleInRad, const glm::vec3& rotationVector);
	void Scale(const glm::vec3& newScale);

private:
	// Warning: ensure this is the only member variable, so we guarantee Transform data/size is always the same from Vertex buffers POV
	glm::mat4 model{ 1.0f };
};



#endif // TRANSFORM_H
