#include "UniformBuffer.h"

#include <glad/glad.h>



UniformBuffer::UniformBuffer(std::vector<unsigned int>& shaderIDs, const std::string& uniformName, const int blockBindingPoint, const unsigned int size)
{
	for (const unsigned int& shaderID : shaderIDs)
	{
		const unsigned int uniformBlockIndex = glGetUniformBlockIndex(shaderID, uniformName.c_str());
		glUniformBlockBinding(shaderID, uniformBlockIndex, blockBindingPoint);
	}

	// Reserve an ID available to be used by the BO as a binding point
	glGenBuffers(1, &rendererID);

	// Bind the BO ID to a target, which specifies our intent to store vertices in it
	glBindBuffer(GL_UNIFORM_BUFFER, rendererID);

	// Allocate memory space (in bytes) to the UBO and store data in it
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);

	// Define the range of the buffer that links to a uniform binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, blockBindingPoint, rendererID, 0, size);
}

UniformBuffer::~UniformBuffer()
{
	// Delete the UBO and free up the ID that was being used by it
	glDeleteBuffers(1, &rendererID);
}

void UniformBuffer::Bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, rendererID);
}

void UniformBuffer::Unbind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::InitSubData(const unsigned int offset, const size_t size, const void* data)
{
	Bind();

	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);

	Unbind();
}