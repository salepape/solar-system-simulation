#include "DataBuffer.h"

#include <glad/glad.h>



DataBuffer::DataBuffer(const void* data, const size_t size, const uint32_t inTarget, const uint32_t usage) :
	target(inTarget)
{
	// Reserve an ID available to be used by the BO as a binding point
	glGenBuffers(1, &rendererID);

	Bind();

	// Allocate memory space (in bytes) to the BO and store data in it
	glBufferData(target, size, data, usage);
}

DataBuffer::~DataBuffer()
{
	Unbind();

	// Delete the VBO and free up the ID that was being used by it
	glDeleteBuffers(1, &rendererID);
}

void DataBuffer::Bind() const
{
	// Bind the BO ID to a target, which specifies our intent to store vertices in it
	glBindBuffer(target, rendererID);
}

void DataBuffer::Unbind() const
{
	glBindBuffer(target, 0);
}

void DataBuffer::SetSubData(const void* data, const size_t size, const uint32_t dataStart)
{
	Bind();

	// Update the data stored in the specified subset of memory space (in bytes), avoiding the reallocation cost
	glBufferSubData(target, dataStart, size, data);

	Unbind();
}

void DataBuffer::SetSubData(const std::vector<SubData>& data, const uint32_t dataStart)
{
	Bind();

	uint32_t offset = dataStart;

	for (const auto& subdata : data)
	{
		// Update the data stored in the specified subset of memory space (in bytes), avoiding the reallocation cost
		glBufferSubData(target, offset, subdata.size, subdata.data);

		offset += static_cast<uint32_t>(subdata.size);
	}

	Unbind();
}