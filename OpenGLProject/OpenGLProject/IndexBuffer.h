#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H



class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, const unsigned int inCount);
	~IndexBuffer();

	// Select the IBO we want to use
	void Bind() const;

	// Unselect the currently used IBO
	void Unbind() const;

	unsigned int GetCount() const { return count; }

private:
	unsigned int rendererID{ 0 };
	unsigned int count{ 0 };
};



#endif // INDEXBUFFER_H