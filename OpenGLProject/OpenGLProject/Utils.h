#pragma once

#include <glm/ext/scalar_constants.hpp>
#include <glm/vec3.hpp>



namespace Utils
{
	const float unitPi = glm::pi<float>();
	const float halfPi = 0.5f * unitPi;
	const float doublePi = 2.0f * unitPi;

	const glm::vec3 forwardVector(0.0f, 0.0f, 1.0f);
	const glm::vec3 rightVector(1.0f, 0.0f, 0.0f);
	const glm::vec3 upVector(0.0f, 1.0f, 0.0f);

	const glm::vec3 whiteColour(1.0f, 1.0f, 1.0f);
	const glm::vec3 whiteColour(1.0f);

	// Compute the data size (in bytes) of each GLSL uniform variable, and their respective offset following the std140 layout base alignement rules 
	constexpr size_t GLSL_SCALAR_SIZE = 4;
	const size_t scalarSize = GLSL_SCALAR_SIZE;
	const size_t vec4Size = 4 * GLSL_SCALAR_SIZE;
	const size_t mat4v4Size = 16 * GLSL_SCALAR_SIZE;
}
