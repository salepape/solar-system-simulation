#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "DataBuffer.h"



class VertexBuffer : public DataBuffer
{
public:
	VertexBuffer(const void* data, const size_t size);
};



#endif // VERTEXBUFFER_H