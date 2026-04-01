#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <cstddef> // std::size_t
#include <cstdint>
#include <string>
#include <vector>

#include "DataBuffer.h"



// Optimise access to a Uniform GLSL variable when used across several GLSL Vertex/Fragment Shader implementations
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