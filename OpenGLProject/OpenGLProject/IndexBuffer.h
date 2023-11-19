#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <glad/glad.h>



class IndexBuffer
{
private:
	unsigned int rendererID{ 0 };
	unsigned int count{ 0 };

public:
	IndexBuffer(const void* data, const unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return count; }
};



#endif // INDEXBUFFER_H