#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <glad/glad.h>
#include <vector>



class VertexBuffer
{
private:
	unsigned int rendererID;

public:
	VertexBuffer(const void* data, const unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void InitSubData(const std::vector<std::vector<float>*>& data);
};



#endif // VERTEXBUFFER_H