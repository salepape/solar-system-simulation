#ifndef RENDERER_H
#define RENDERER_H

class IndexBuffer;
class VertexArray;



class Renderer
{
public:
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

	// For belts
	void DrawInstanced(const VertexArray& vao, const unsigned int iboCount, const unsigned int count) const;

	// For skybox and text
	void Draw(const VertexArray& vao, const IndexBuffer& ibo, const unsigned int mode, const unsigned int count) const;

	// For meshes
	void Draw(const VertexArray& vao, const IndexBuffer& ibo) const;

	// For orbits
	void Draw(const VertexArray& vao, const unsigned int mode, const unsigned int count) const;
};



#endif // RENDERER_H