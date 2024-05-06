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

	void EnableDepthTesting() const;
	void DisableDepthTesting() const;

	// Be sure all meshes are defined in a counter-clockwise fashion
	void EnableFaceCulling() const;
	void DisableFaceCulling() const;

	// Enable OpenGL states for blending to make texts rendered correctly
	void EnableBlending() const;
	void DisableBlending() const;

	// Set the function that will be used to compare each pixel depth value with the one stored in buffer
	void SetDepthFctToEqual() const;
	void SetDepthFctToLess() const;

	// Draw arrays (e.g. for Orbit, Skybox and Text instances)
	void Draw(const VertexArray& vao, const unsigned int mode, const unsigned int count) const;

	// Draw elements using an IBO (e.g. for Mesh instances)
	void Draw(const VertexArray& vao, const IndexBuffer& ibo) const;

	// Draw elements (e.g. for Belt instances)
	void DrawInstances(const VertexArray& vao, const unsigned int iboCount, const unsigned int instanceCount) const;
};



#endif // RENDERER_H