#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <glad.h>
#include <vector>



struct SubData
{
	const void* data{ nullptr };
	const size_t sizeInBytes{ 0 };
};

class DataBuffer
{
public:
	// Default constructor needed for UniformBuffer constructor not using DataBuffer user-defined one
	DataBuffer() = default;
	DataBuffer(const void* data, const size_t sizeInBytes, const uint32_t inTarget, const uint32_t usage = GL_STATIC_DRAW);
	~DataBuffer();

	// Select the VBO we want to use
	void Bind() const;

	// Unselect the currently used VBO
	void Unbind() const;

	// Set data in the memory region starting after the indicated amount of bytes
	void SetSubData(const void* data, const size_t sizeInBytes, const uint32_t dataStart = 0);
	void SetSubData(const std::vector<SubData>& data, const uint32_t dataStart = 0);

protected:
	uint32_t rendererID{ 0 };
	uint32_t target{ 0 };
};



#endif // DATABUFFER_H