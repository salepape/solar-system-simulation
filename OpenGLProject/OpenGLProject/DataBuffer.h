#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <iostream>
#include <vector>



struct SubData
{
	const void* data{ nullptr };
	const size_t size{ 0 };
};

class DataBuffer
{
public:
	DataBuffer();
	DataBuffer(const void* data, const size_t size, const int inTarget);
	virtual ~DataBuffer();

	// Select the VBO we want to use
	void Bind() const;

	// Unselect the currently used VBO
	void Unbind() const;

	void InitSubData(const std::vector<SubData>& data, const unsigned int dataOffset = 0);

protected:
	unsigned int rendererID{ 0 };
	int target{ 0 };
};



#endif // DATABUFFER_H