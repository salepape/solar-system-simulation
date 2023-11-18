#include "VertexBuffer.h"



VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
	// GENERATION VBO - Generate 1 BO storing its ID / name in the provided array within graphics memory
	glGenBuffers(1, &rendererID);
	// BINDING VBO - Bind BO to the OpenGL BO target we want (here the one of a vertex, called GL_ARRAY_BUFFER, hence VBO variable name)
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	// COPY - Allocate memory and store data into the currently bound BO memory
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	// CLEAN VBO - Free the resource once it've outlived its purpose
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

void VertexBuffer::InitSubData(std::vector<std::vector<float>*>& data)
{
	unsigned int offset = 0;
	for (size_t i = 0; i < data.size(); ++i)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(float) * data[i]->size(), &(*data[i])[0]);
		offset += sizeof(float) * data[i]->size();
	}
}