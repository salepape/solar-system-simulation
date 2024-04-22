#include "IndexBuffer.h"

#include <glad/glad.h>



IndexBuffer::IndexBuffer(const void* data, const unsigned int inCount) : count(inCount), 
DataBuffer(data, static_cast<size_t>(inCount * sizeof(unsigned int)), GL_ELEMENT_ARRAY_BUFFER)
{

}