#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



class VertexArray 
{
private:
	unsigned int rendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vbo,  const VertexBufferLayout& layout);
	void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout, std::vector<unsigned int> sizeofs);
	void AddInstancedBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};

