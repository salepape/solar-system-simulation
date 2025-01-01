#include "UniformBuffer.h"

#include <glad/glad.h>

uint32_t UniformBuffer::blockBindingPoint = 0;



UniformBuffer::UniformBuffer(const std::vector<uint32_t>& shaderIDs, const std::string& inUniformName, const size_t sizeInBytes)
{
	target = GL_UNIFORM_BUFFER;
	uniformName = inUniformName;

	for (const uint32_t& shaderID : shaderIDs)
	{
		const uint32_t uniformBlockIndex = glGetUniformBlockIndex(shaderID, uniformName.c_str());
		glUniformBlockBinding(shaderID, uniformBlockIndex, blockBindingPoint);
	}

	// Reserve an ID available to be used by the UBO as a binding point
	glGenBuffers(1, &rendererID);

	// Bind the UBO ID to a target, which specifies our intent to store vertices in it
	glBindBuffer(target, rendererID);

	// @todo - Move functions below in proper class to avoid having the size as parameter in ResourceLoader?

	// Allocate memory space (in bytes) to the UBO and store data in it
	glBufferData(target, sizeInBytes, nullptr, GL_STATIC_DRAW);

	// Define the range of the buffer that links to a uniform binding point
	glBindBufferRange(target, blockBindingPoint++, rendererID, 0, sizeInBytes);
}