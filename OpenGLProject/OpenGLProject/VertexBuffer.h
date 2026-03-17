#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <cstddef> // std::size_t
#include <glad.h>

#include "DataBuffer.h"



class VertexBuffer : public DataBuffer
{
public:
	VertexBuffer(const void* data, const std::size_t sizeInBytes, const int usage = GL_STATIC_DRAW);
};



#endif // VERTEXBUFFER_H