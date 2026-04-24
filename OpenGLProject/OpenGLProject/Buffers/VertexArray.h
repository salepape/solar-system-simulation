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

	// Parameterise VAO so the VBO is interpreted correctly for GLSL Shader attributes. Warning: require VAO/VBO to be bound beforehand 
	void RegisterVertexBufferLayout(const VertexBufferLayout& layout);

	// Parameterise VAO so the VBO is interpreted correctly for GLSL Shader attributes in an instancing context. Warning: require VAO/VBO to be bound beforehand 
	void RegisterInstancingVertexBufferLayout(const VertexBufferLayout& layout) const;

private:
	unsigned int rendererID{ 0 };
};



#endif // VERTEXARRAY_H