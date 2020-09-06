#pragma once

#include <glad/glad.h>
#include <vector>



class VertexBuffer
{
private:
	unsigned int rendererID;

public:
	VertexBuffer(const void * data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void InitSubData(std::vector<std::vector<float>*>&);
};