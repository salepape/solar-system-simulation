#ifndef RENDERER_H
#define RENDERER_H

#include <glm/mat4x4.hpp>

class IndexBuffer;
class Shader;
class VertexArray;



// Static wrapper of all OpenGL functions allowing to draw or have a visual change on screen
class Renderer
{
public:
	static bool IsOpenGLContextActive();

	// Clear the depth buffer
	static void Clear();

	static void EnableDepthTesting();
	static void DisableDepthTesting();

	// Be sure all meshes are defined in a counter-clockwise fashion
	static void EnableFaceCulling();
	static void DisableFaceCulling();

	// Enable OpenGL states for blending to make texts rendered correctly
	static void EnableBlending();
	static void DisableBlending();

	// Set the function that will be used to compare each pixel depth value with the one stored in buffer
	static void SetDepthFctToEqual();
	static void SetDepthFctToLess();

	// @todo - Create a Movement Component and move this method out of it
	// Called on a per-frame basis to update the Transform.
	// Shader should already be enabled in each Scene Entity child Render() method prior to call this one. Uniform to be updated in child classes
	static void SetModelMatrixVUniform(const Shader& shader, const glm::mat4& modelMatrix);

	// Draw arrays (e.g. for Orbit, Skybox and Text instances)
	static void Draw(const VertexArray& vao, const unsigned int mode, const int32_t startIndex, const unsigned int count);

	// Draw elements using an IBO (e.g. for Mesh instances)
	static void Draw(const VertexArray& vao, const IndexBuffer& ibo);

	// Draw elements (e.g. for Belt instances 'Rock' Model instancing)
	static void DrawInstances(const VertexArray& vao, const unsigned int iboCount, const unsigned int instanceCount);
};



#endif // RENDERER_H