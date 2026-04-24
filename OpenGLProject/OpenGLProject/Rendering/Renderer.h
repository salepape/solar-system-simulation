#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>
#include <glm/mat4x4.hpp>

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

	// Render a primitive without indices (e.g. for Orbit, Skybox and 2D Quad instance)
	static void Draw(const VertexArray& vao, const unsigned int mode, const int32_t startIndex, const int32_t count);

	// Render a primitive with indices (e.g. for Mesh instance)
	static void Draw(const VertexArray& vao, const unsigned int mode, const int32_t count, const void* offsetInBytes);

	// Render primitives without indices using instancing (e.g. for 'Rock' Models in Belt instance)
	static void DrawInstances(const VertexArray& vao, const unsigned int mode, const int32_t startIndex, const int32_t count, const int32_t instanceCount);
};



#endif // RENDERER_H