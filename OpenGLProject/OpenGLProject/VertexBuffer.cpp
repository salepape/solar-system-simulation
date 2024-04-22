#include "VertexBuffer.h"

#include <glad/glad.h>



VertexBuffer::VertexBuffer(const void* data, const size_t size):
	DataBuffer(data, size, GL_ARRAY_BUFFER)
{

}