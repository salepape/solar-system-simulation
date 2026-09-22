#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>
#include <glm/mat4x4.hpp>

class Shader;
class Transform;



// Wrapper of all OpenGL functions allowing to draw or have a visual change on screen
namespace Renderer
{
	bool IsOpenGLContextActive();

	// Clear all OpenGL buffer targets used
	void ClearBufferTargets();

	// @todo - Same as depth culling?
	void EnableDepthTesting();
	void DisableDepthTesting();

	// @todo - Add Collision Component to all celestial bodies, as back (= inner) face culling active
	// Discard every draw call for any geometry (e.g. inner part of spheres) not visible from the user's point of view
	// Warning: all vertices need to be written in a counter-clockwise fashion
	void EnableBackFaceCulling();
	void DisableBackFaceCulling();

	// Enable OpenGL states for blending to make texts rendered correctly
	void EnableBlending();
	void DisableBlending();

	// @todo - Frustrum culling

	// Set the function that will be used to compare each pixel depth value with the one stored in buffer
	void SetDepthFctToEqual();
	void SetDepthFctToLess();

	// @todo - Drag to a Movement Component class?
	// Update the Transform value of a Scene Entity on a per-frame basis, for the provided shader.
	// Warning: shader needs to be enabled prior to this call, and contain the 'Model' parameter in GLSL code
	void SetTransformVUniform(const Shader& shader, const Transform& transform);

	// Update the material alpha parameter value on a per-frame basis, for the provided shader.
	// Warning: shader needs to be enabled prior to this call, and contain the 'Alpha' material parameter in GLSL code
	void SetAlphaFUniform(const Shader& shader, const float alpha);

	// Render a primitive without indices (e.g. for Orbit, Skybox and 2D Quad instance).
	// Warning: VAO must be bound prior to this call, and unbound afterwards
	void Draw(const unsigned int mode, const int32_t startIndex, const int32_t count);

	// Render a primitive with indices (e.g. for Mesh instance).
	// Warning: VAO must be bound prior to this call, and unbound afterwards
	void Draw(const unsigned int mode, const int32_t count, const void* offsetInBytes);

	// Render primitives without indices using instancing (e.g. for 'Rock' Models in Belt instance) - Warning: VAO must be bound prior to this call, and unbound afterwards
	void DrawInstances(const unsigned int mode, const int32_t startIndex, const int32_t count, const int32_t instanceCount);
};



#endif // RENDERER_H
