#include "VertexBuffer.h"

#include <glad/glad.h>



VertexBuffer::VertexBuffer(const void* data, const unsigned int size)
{
	// Reserve an ID available to be used by the BO as a binding point
	glGenBuffers(1, &rendererID);

	// Bind the BO ID to a target, which specifies our intent to store vertices in it
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);

	// Allocate memory space (in bytes) to the VBO and store data in it
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	// Delete the VBO and free up the ID that was being used by it
	glDeleteBuffers(1, &rendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::InitSubData(const std::vector<SubData>& data)
{
	unsigned int offset = 0;

	for (const auto& subdata : data)
	{
		// Update the data stored in the specified subset of memory space (in bytes), avoiding the reallocation cost
		glBufferSubData(GL_ARRAY_BUFFER, offset, subdata.size, subdata.data);

		offset += static_cast<unsigned int>(subdata.size);
	}
}