#ifndef RENDERER_H
#define RENDERER_H

class IndexBuffer;
class VertexArray;



class Renderer
{
public:
	// Default constructor needed since we deleted the copy constructor
	Renderer() = default;
	Renderer(const Renderer&) = delete;

	// Clear the depth buffer
	void Clear() const;

	void DepthTest() const;

	// Enable OpenGL states for blending to make texts rendered correctly
	void Blend() const;

	// Change depth function so that depth test passes when values are equal to depth buffer's content
	void DepthEqual() const;

	// Change depth function so that depth test passes when values are inferior to depth buffer's content
	void DepthLess() const;

	// Draw arrays (e.g. for Orbit, Skybox and Text instances)
	void Draw(const VertexArray& vao, const unsigned int mode, const unsigned int count) const;

	// Draw elements using an IBO (e.g. for Mesh instances)
	void Draw(const VertexArray& vao, const IndexBuffer& ibo) const;

	// Draw elements (e.g. for Belt instances)
	void DrawInstances(const VertexArray& vao, const unsigned int iboCount, const unsigned int instanceCount) const;
};



#endif // RENDERER_H