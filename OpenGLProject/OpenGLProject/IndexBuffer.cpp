#include "IndexBuffer.h"

#include <glad/glad.h>



IndexBuffer::IndexBuffer(const void* data, const unsigned int inCount) : count(inCount)
{
	// Reserve an ID available to be used by the BO as a binding point
	glGenBuffers(1, &rendererID);

	// Bind the BO ID to a target, which specifies our intent to store indexes in it
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);

	// Allocate memory space (in bytes) to the IBO and store data in it
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	// Delete the VBO and free up the ID that was being used by it
	glDeleteBuffers(1, &rendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}