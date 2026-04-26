#include "VertexBuffer.h"

#include <glad/glad.h>



VertexBuffer::VertexBuffer(const void* data, const std::size_t sizeInBytes, const int usage) : DataBuffer(data, sizeInBytes, GL_ARRAY_BUFFER, usage)
{

}
