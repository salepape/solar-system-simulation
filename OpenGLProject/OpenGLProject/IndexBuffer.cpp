#include "IndexBuffer.h"

#include <glad/glad.h>



IndexBuffer::IndexBuffer(const void* data, const uint32_t inCount) : count(inCount),
DataBuffer(data, static_cast<size_t>(inCount) * sizeof(uint32_t), GL_ELEMENT_ARRAY_BUFFER)
{

}