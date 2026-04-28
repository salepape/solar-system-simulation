#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

class VertexBuffer;
class VertexBufferLayout;



// Collection of OpenGL States to understand which GLSL Shader memory layout to apply to which VBO (resulting in the definition of GLSL Vertex Attributes as expected).
// No VBO/IBO bindings needed when using VAO bindings: save states as well. Using them to be in line modern OpenGL way of accessing info.
// Note: binding of VAOs is a global state, and decide when subsequent OpenGL calls will work
class VertexArray
{
public:
	// Constructor of VAO includes a binding, so no need to call the method separately just after instantiation
	VertexArray();
	~VertexArray();

	// Select the VAO we want to activate in the array
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



#endif // VERTEX_ARRAY_H
