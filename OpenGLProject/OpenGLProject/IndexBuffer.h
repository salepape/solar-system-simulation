#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "DataBuffer.h"



class IndexBuffer : public DataBuffer
{
public:
	IndexBuffer(const void* data, const uint32_t inCount);

	uint32_t GetCount() const { return count; }

private:
	uint32_t count{ 0 };
};



#endif // INDEXBUFFER_H