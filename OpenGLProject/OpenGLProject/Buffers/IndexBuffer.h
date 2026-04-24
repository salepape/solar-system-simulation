#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <cstdint>

#include "DataBuffer.h"



class IndexBuffer : public DataBuffer
{
public:
	IndexBuffer(const void* data, const int32_t inCount);

	int32_t GetCount() const { return count; }

private:
	int32_t count{ 0 };
};



#endif // INDEXBUFFER_H