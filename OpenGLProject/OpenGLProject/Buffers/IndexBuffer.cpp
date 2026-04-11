#include "IndexBuffer.h"

#include <cstddef> // std::size_t
#include <glad/glad.h>



IndexBuffer::IndexBuffer(const void* data, const uint32_t inCount) : DataBuffer(data, static_cast<std::size_t>(inCount) * sizeof(uint32_t), GL_ELEMENT_ARRAY_BUFFER),
count(inCount)
{

}