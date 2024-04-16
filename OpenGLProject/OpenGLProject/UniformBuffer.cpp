#include "UniformBuffer.h"

#include <glad/glad.h>



UniformBuffer::UniformBuffer(std::vector<unsigned int>& shaderIDs, const std::string& uniformName, const int blockBindingPoint, const size_t size)
{
	target = GL_UNIFORM_BUFFER;

	for (const unsigned int& shaderID : shaderIDs)
	{
		const unsigned int uniformBlockIndex = glGetUniformBlockIndex(shaderID, uniformName.c_str());
		glUniformBlockBinding(shaderID, uniformBlockIndex, blockBindingPoint);
	}

	// Reserve an ID available to be used by the BO as a binding point
	glGenBuffers(1, &rendererID);

	// Bind the BO ID to a target, which specifies our intent to store vertices in it
	glBindBuffer(target, rendererID);

	// Allocate memory space (in bytes) to the UBO and store data in it
	glBufferData(target, size, nullptr, GL_STATIC_DRAW);

	// Define the range of the buffer that links to a uniform binding point
	glBindBufferRange(target, blockBindingPoint, rendererID, 0, size);
}

UniformBuffer::~UniformBuffer()
{

}