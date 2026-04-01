#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <cstddef> // std::size_t
#include <cstdint>
#include <string>
#include <vector>

#include "DataBuffer.h"



// Store the reference to a GLSL Uniform that is shared across several GLSL Vertex/Fragment Shaders, to allow for a single call to update its value in all of them
class UniformBuffer : public DataBuffer
{
public:
	// uniformName can either be a single uniform or a struct provided using layout std140 in shader
	UniformBuffer(const std::vector<uint32_t>& shaderIDs, const std::string& inUniformName, const std::size_t sizeInBytes);

	const std::string& GetUniformName() const { return uniformName; }

private:
	std::string uniformName;
	static uint32_t blockBindingPoint;
};



#endif // UNIFORMBUFFER_H