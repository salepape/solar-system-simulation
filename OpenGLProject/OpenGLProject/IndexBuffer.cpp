#include "IndexBuffer.h"



IndexBuffer::IndexBuffer(const void * data, unsigned int count) : count(count)
{
	if (sizeof(unsigned int) != sizeof(GLuint))
		return;

	// GENERATION IBO - Generate 1 BO storing its ID / name in the provided array within graphics memory
	glGenBuffers(1, &rendererID);
	// BINDING VBO - Bind BO to the OpenGL BO target we want (here the one of a vertex, called GL_ARRAY_BUFFER, hence VBO variable name)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	// COPY - Allocate memory and store data into the currently bound BO memory
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	// CLEAN IBO - Free the resource once it've outlived its purpose
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