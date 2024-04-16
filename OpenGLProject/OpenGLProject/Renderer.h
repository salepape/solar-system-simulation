#ifndef RENDERER_H
#define RENDERER_H

class IndexBuffer;
class VertexArray;



class Renderer
{
public:
	// @todo - Add default constructor/destructor?

	// Clear the depth buffer
	void Clear() const;

	void DepthTest() const;

	void DepthClamp() const;

	// Enable OpenGL states for blending to make texts rendered correctly
	void Blend() const;

	// Trim down polygon count (but we cannot "visit" spheres interior anymore)
	void CullFace() const;

	// Change depth function so that depth test passes when values are equal to depth buffer's content
	void DepthEqual() const;

	// Change depth function so that depth test passes when values are inferior to depth buffer's content
	void DepthLess() const;

	// Set "RGB + alpha" coefficients for background color
	void SetColor();

	// Draw arrays (e.g. for Orbit, Skybox and Text instances)
	void Draw(const VertexArray& vao, const unsigned int mode, const unsigned int count) const;

	// Draw elements using an IBO (e.g. for Mesh instances)
	void Draw(const VertexArray& vao, const IndexBuffer& ibo) const;

	// Draw elements (e.g. for Belt instances)
	void DrawInstances(const VertexArray& vao, const unsigned int iboCount, const unsigned int count) const;
};



#endif // RENDERER_H