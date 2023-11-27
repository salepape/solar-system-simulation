#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



class VertexArray
{
private:
	unsigned int rendererID;

public:
	VertexArray();
	~VertexArray();

	// Select the VAO we want to use
	void Bind() const;

	// Unselect the currently used VAO
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
	void AddInstancedBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
};



#endif // VERTEXARRAY_H