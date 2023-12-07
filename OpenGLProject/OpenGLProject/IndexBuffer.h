#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H



class IndexBuffer
{
private:
	unsigned int rendererID{ 0 };
	unsigned int count{ 0 };

public:
	IndexBuffer(const unsigned int* data, const unsigned int count);
	~IndexBuffer();

	// Select the IBO we want to use
	void Bind() const;

	// Unselect the currently used IBO
	void Unbind() const;

	inline unsigned int GetCount() const { return count; }
};



#endif // INDEXBUFFER_H