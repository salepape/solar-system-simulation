#pragma once

#include <vector>
#include <glad.h>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetTypeSize(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:	return 4;
		default:		return 0;
		}
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:
	VertexBufferLayout();
	~VertexBufferLayout();

	template<typename T> 
	void Push(unsigned int count)
	{

	}

	template <>
	void Push<float>(unsigned int count) 
	{
		elements.push_back({GL_FLOAT, count, GL_FALSE});
		stride += count * VertexBufferElement::GetTypeSize(GL_FLOAT);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return elements; }

	inline unsigned int GetStride() const { return stride; }
};
