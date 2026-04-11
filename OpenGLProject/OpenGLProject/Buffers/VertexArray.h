#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

class VertexBuffer;
class VertexBufferLayout;



class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	// Select the VAO we want to use
	void Bind() const;

	// Unselect the currently used VAO
	void Unbind() const;

	void AddBuffer(const VertexBufferLayout& layout);
	void AddInstancedBuffer(const VertexBufferLayout& layout) const;

private:
	unsigned int rendererID{ 0 };
};



#endif // VERTEXARRAY_H