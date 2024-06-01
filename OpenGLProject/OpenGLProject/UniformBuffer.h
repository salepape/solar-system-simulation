#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <string>
#include <vector>

#include "DataBuffer.h"



class UniformBuffer : public DataBuffer
{
public:
	// uniformName can either be a single uniform or a struct provided using layout std140 in shader
	UniformBuffer(const std::vector<uint32_t>& shaderIDs, const std::string& inUniformName, const size_t size);

	const std::string& GetUniformName() const { return uniformName; }

private:
	std::string uniformName;
	static uint32_t blockBindingPoint;
};



#endif // UNIFORMBUFFER_H