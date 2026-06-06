#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstddef>		// std::size_t
#include <functional>	// std::reference_wrapper
#include <optional>



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

struct RotationMatrix
{
	glm::mat3 matrix;

	RotationMatrix(const glm::vec3& rightVector, const glm::vec3& upVector, const glm::vec3& forwardVector) :
		matrix(rightVector, upVector, forwardVector) {
	}

	const glm::vec3 GetRightVector() const { return matrix[0]; }
	const glm::vec3 GetUpVector() const { return matrix[1]; }
	const glm::vec3 GetForwardVector() const { return matrix[2]; }
};

// Correspond to the Scene Transform (position, rotation, scale) of the Scene Entity
class Transform
{
public:
	Transform() = default;
	Transform(const glm::vec3& position, const EulerAngles& rotation);

	const glm::mat4& Get() const { return model; }
	void Set(const glm::mat4& inModel) { model = inModel; }

	// Get position out of Model matrix
	glm::vec3 GetPosition() const;

	// Store position directly in Model matrix
	void SetPosition(const glm::vec3& inPosition);

	// Get rotation info out of local cache
	const EulerAngles& GetRotation() const { return rotation; }

	// Store rotation direcly in Model matrix (per column) and update Euler Angles by deduction
	void SetRotation(const RotationMatrix& inRotation);

	// Overwrite Euler Angles and update LookAt vectors accordingly
	void SetRotation(const EulerAngles& inRotationInRad);

	// Increment Euler Angles and update LookAt vectors accordingly
	void UpdateRotation(const EulerAngles& newAngles);

	void Reset();

	// @todo - Methods below to be moved to Movement Component or similar?

	// Translate model by distance travelled (do not overwrite existing model matrix)
	void Translate(const glm::vec3& newPosition);

	// Rotate model by angle travelled around axis (do not overwrite existing model matrix)
	void Rotate(const float angleInRad, const glm::vec3& rotationAxis);

	void Scale(const glm::vec3& newScale);

	static constexpr size_t GetMatrixSizeInBytes() { return sizeof(model); }
	static constexpr size_t GetColumnSizeInBytes() { return static_cast<size_t>(sizeof(model) * 0.25f); }

	const glm::vec3& GetUpVector() const { return upVector; }
	const glm::vec3& GetRightVector() const { return rightVector; }
	const glm::vec3& GetForwardVector() const { return forwardVector; }

private:
	// Warning: ensure this is the only member variable, so we guarantee Transform data/size is always the same from Vertex buffers POV
	glm::mat4 model{ 1.0f };

	// Euler angles [in radians] for embedded LookAt vectors (simpler to keep track as is, and save expensive computations per frame)
	EulerAngles rotation;

	glm::vec3 forwardVector{ 0.0f, 0.0f, -1.0f };
	glm::vec3 rightVector{ 0.0f, 1.0f, 0.0f };
	glm::vec3 upVector{ 1.0f, 0.0f, 0.0f };

	// Compute new LookAt vectors Forward, Right, Up, before updating Model matrix
	void UpdateLookAtVectors();

	// Update Model matrix on a per-column basis (i.e. by LookAt vectors, provided or not)
	void UpdateModelMatrix(std::optional<std::reference_wrapper<const RotationMatrix>> inRotation = std::nullopt);
};



#endif // TRANSFORM_H
