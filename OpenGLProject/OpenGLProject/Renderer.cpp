#include "Renderer.h"



// Clear the depth buffer 
void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Conduct a depth test
void Renderer::DepthTest() const
{
	glEnable(GL_DEPTH_TEST);
}

// Conduct a depth clamp
void Renderer::DepthClamp() const
{
	glEnable(GL_DEPTH_CLAMP);
}

// OpenGL states enabled to make texts rendering correctly
void Renderer::Blend() const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Trim down polygon count (but we cannot "visit" spheres interior anymore)
void Renderer::CullFace() const
{
	glEnable(GL_CULL_FACE);
}

// Change depth function so that depth test passes when values are equal to depth buffer's content
void Renderer::DepthEqual() const
{
	glDepthFunc(GL_LEQUAL);
}

// Change depth function so that depth test passes when values are inferior to depth buffer's content
void Renderer::DepthLess() const
{
	glDepthFunc(GL_LESS);
}

// Set color when color buffers are cleared
void Renderer::SetColor()
{
	// Set "RGB + alpha" coefficients for background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}




// belt
void Renderer::DrawInstanced(const VertexArray& vao, unsigned int iboCount, unsigned int count) const
{
	vao.Bind();
	//ibo.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, iboCount, GL_UNSIGNED_INT, 0, count);
	//ibo.Unbind();
	vao.Unbind();
}

// skybox & text
void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, unsigned int mode, unsigned int count) const
{
	vao.Bind();
	ibo.Bind();
	glDrawArrays(mode, 0, count);
	ibo.Unbind();
	vao.Unbind();
}

// mesh & spheres
void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo) const
{
	vao.Bind();
	ibo.Bind();
	glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
	ibo.Unbind();
	vao.Unbind();
}

// orbit
void Renderer::Draw(const VertexArray& vao, unsigned int mode, unsigned int count) const
{
	vao.Bind();
	//ibo.Bind();
	glDrawArrays(mode, 0, count);
	//ibo.Unbind();
	vao.Unbind();
}


