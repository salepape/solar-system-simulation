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

	void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

	// @todo - See if we need to support other types than mat4 (with vec4 as an offset) for instancing
	void AddInstancedBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

private:
	unsigned int rendererID{ 0 };
};



#endif // VERTEXARRAY_H