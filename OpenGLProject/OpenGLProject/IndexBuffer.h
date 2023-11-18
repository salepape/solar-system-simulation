#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <glad/glad.h>



class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int count;

public:
	IndexBuffer(const void * data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return count; }
};



#endif // INDEXBUFFER_H