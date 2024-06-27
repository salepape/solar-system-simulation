#include "VertexBuffer.h"

#include <glad/glad.h>



VertexBuffer::VertexBuffer(const void* data, const size_t size, const int usage) : DataBuffer(data, size, GL_ARRAY_BUFFER, usage)
{

}