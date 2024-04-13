#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <iostream>
#include <vector>



struct SubData
{
	const void* data{ nullptr };
	unsigned int size{ 0 };
};

class VertexBuffer
{
public:
	VertexBuffer(const void* data, const unsigned int size);
	~VertexBuffer();

	// Select the VBO we want to use
	void Bind() const;

	// Unselect the currently used VBO
	void Unbind() const;

	void InitSubData(const std::vector<SubData>& data);

private:
	unsigned int rendererID{ 0 };
};



#endif // VERTEXBUFFER_H