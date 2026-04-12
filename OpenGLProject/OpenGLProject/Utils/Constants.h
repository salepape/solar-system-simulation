#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef> // std::size_t
#include <glm/ext/scalar_constants.hpp>
#include <glm/vec3.hpp>

// Store the size (in bytes) of usual GLSL variable types
// glm::vec3 have to be converted to GLSL vec4 according std140 layout base alignement rules (offset). Also glm:vec4 does not support glm::value_ptr
namespace GLSLConstants
{
	// Correspond to 1 * Int size
	constexpr std::size_t scalarSizeInBytes = 4;

	// Correspond to 4 * Int size
	constexpr std::size_t vec4SizeInBytes = 16;

	// Correspond to 16 * Int size
	constexpr std::size_t mat4v4SizeInBytes = 64;
}

namespace GLMConstants
{
	const float unitPi = glm::pi<float>();
	const float halfPi = 0.5f * unitPi;
	const float doublePi = 2.0f * unitPi;

	const glm::vec3 zeroVector(0.0f);

	const glm::vec3 forwardVector(0.0f, 0.0f, 1.0f);
	const glm::vec3 rightVector(1.0f, 0.0f, 0.0f);
	const glm::vec3 upVector(0.0f, 1.0f, 0.0f);

	const glm::vec3 whiteColour(1.0f);
}



#endif // CONSTANTS_H
