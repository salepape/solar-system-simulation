#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <vector>



struct SubData
{
	const void* data{ nullptr };
	const size_t size{ 0 };
};

class DataBuffer
{
public:
	// Default constructor needed for UniformBuffer constructor not using DataBuffer user-defined one
	DataBuffer() = default;
	DataBuffer(const void* data, const size_t size, const uint32_t inTarget);
	~DataBuffer();

	// Select the VBO we want to use
	void Bind() const;

	// Unselect the currently used VBO
	void Unbind() const;

	// Set data in the memory region starting after the indicated amount of bytes
	void InitSubData(const std::vector<SubData>& data, const uint32_t dataStart = 0);

protected:
	uint32_t rendererID{ 0 };
	uint32_t target{ 0 };
};



#endif // DATABUFFER_H