#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <string>
#include <vector>

#include "DataBuffer.h"



class UniformBuffer : public DataBuffer
{
public:
	UniformBuffer(std::vector<unsigned int>& shaderIDs, const std::string& uniformName, const int blockBindingPoint, const size_t size);
	~UniformBuffer() override;
};



#endif // UNIFORMBUFFER_H