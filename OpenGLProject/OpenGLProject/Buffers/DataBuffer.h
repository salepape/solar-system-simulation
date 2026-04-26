#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include <glad/glad.h>

#include <cstdint>
#include <cstddef> // std::size_t
#include <vector>



struct SubData
{
	const void* data{ nullptr };
	std::size_t sizeInBytes{ 0 };
};

// Represent a contiguous block of memory containing unformatted data, called Buffer Object (BO), that OpenGL can access and modify.
// Common class of all OpenGL buffer Objects of use (i.e. VBO, IBO or EBO, VAO, UBO). BO should never been instantiated outside of its children.
// Note: binding of BOs is a global OpenGL state, and decide when subsequent OpenGL calls will work
class DataBuffer
{
public:
	// Constructor of BO includes a binding and a data setter, so no need to call these methods separately just after instantiation of children
	DataBuffer() = default;
	DataBuffer(const void* data, const std::size_t sizeInBytes, const uint32_t inTarget, const uint32_t usage = GL_STATIC_DRAW);
	~DataBuffer();

	// Select an OpenGL Target listed in the Context so we make the State associated with the BO current
	void Bind() const;

	// Clear the specified OpenGL Target for the current BO, letting it free to be bound to any other target later on
	void Unbind() const;

	// Allocate the needed number of data bytes in the memory region of the GPU (after a certain byte offset, if needed)
	void SetSubData(const void* data, const std::size_t sizeInBytes, const uint32_t dataStart = 0) const;
	void SetSubData(const std::vector<SubData>& data, const uint32_t dataStart = 0) const;

protected:
	uint32_t rendererID{ 0 };
	uint32_t target{ 0 };
};



#endif // DATA_BUFFER_H
