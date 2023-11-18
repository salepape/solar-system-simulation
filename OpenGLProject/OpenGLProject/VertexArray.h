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

	void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
	void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout, const std::vector<unsigned int> sizeofs);
	void AddInstancedBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};



#endif // VERTEXARRAY_H