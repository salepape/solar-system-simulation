#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <iostream>
#include <vector>



class UniformBuffer
{
public:
	UniformBuffer(std::vector<unsigned int>& shaderIDs, const std::string& uniformName, const int blockBindingPoint, const unsigned int size);
	~UniformBuffer();

	// Select the UBO we want to use
	void Bind() const;

	// Unselect the currently used UBO
	void Unbind() const;

	void InitSubData(const unsigned int offset, const size_t size, const void* data);

private:
	unsigned int rendererID{ 0 };
};



#endif // UNIFORMBUFFER_H