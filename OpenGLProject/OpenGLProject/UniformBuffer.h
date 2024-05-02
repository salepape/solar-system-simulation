#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <string>
#include <vector>

#include "DataBuffer.h"



class UniformBuffer : public DataBuffer
{
public:
	UniformBuffer(const std::vector<uint32_t>& shaderIDs, const std::string& uniformName, const size_t size);

private:
	static int blockBindingPoint;
};



#endif // UNIFORMBUFFER_H