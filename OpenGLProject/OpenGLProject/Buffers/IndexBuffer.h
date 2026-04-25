#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <cstdint>

#include "DataBuffer.h"



// Array of numbers allowing to index vertex data in VBO. IBO is also known as Element Buffer Object (EBO).
// No need to store same vertex data if defined triangles share same vertices, give vertex indices matching common Vertex component instead.
// Warning: binding of IBOs is a global OpenGL state: should always be bound AFTER VAO, and unbound AFTER as well
class IndexBuffer : public DataBuffer
{
public:
	// Constructor of IBO includes a binding and a data setter, so no need to call these methods separately just after instantiation
	IndexBuffer(const void* data, const int32_t inCount);

	// Return amount of indices stored in the IBO (should correspond to value of 'indices.size()' in Mesh class definitions
	int32_t GetCount() const { return count; }

private:
	int32_t count{ 0 };
};



#endif // INDEXBUFFER_H