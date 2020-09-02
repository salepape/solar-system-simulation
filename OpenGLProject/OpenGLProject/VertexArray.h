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
	void AddBuffer2(const VertexBuffer& vbo, const VertexBufferLayout& layout, std::vector<unsigned int> sizeofs);
	void AddInstancedBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

	void Bind() const;
	//void Bind(unsigned int arrayID) const;		// when we need mesh rendererID to be linked
	void Unbind() const;

	//inline const unsigned int GetID() const { return rendererID; }
};