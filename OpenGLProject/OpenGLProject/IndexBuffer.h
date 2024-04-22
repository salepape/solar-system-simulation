#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "DataBuffer.h"



class IndexBuffer : public DataBuffer
{
public:
	IndexBuffer(const void* data, const unsigned int inCount);

	unsigned int GetCount() const { return count; }

private:
	unsigned int count{ 0 };
};



#endif // INDEXBUFFER_H