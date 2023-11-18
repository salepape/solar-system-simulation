#include "Renderer.h"



void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DepthTest() const
{
	glEnable(GL_DEPTH_TEST);
}

void Renderer::DepthClamp() const
{
	glEnable(GL_DEPTH_CLAMP);
}

void Renderer::Blend() const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::CullFace() const
{
	glEnable(GL_CULL_FACE);
}

void Renderer::DepthEqual() const
{
	glDepthFunc(GL_LEQUAL);
}

void Renderer::DepthLess() const
{
	glDepthFunc(GL_LESS);
}

void Renderer::SetColor()
{
	// Set "RGB + alpha" coefficients for background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}




// belt
void Renderer::DrawInstanced(const VertexArray& vao, const unsigned int iboCount, const unsigned int count) const
{
	vao.Bind();
	//ibo.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, iboCount, GL_UNSIGNED_INT, 0, count);
	//ibo.Unbind();
	vao.Unbind();
}

// skybox & text
void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const unsigned int mode, const unsigned int count) const
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
void Renderer::Draw(const VertexArray& vao, const unsigned int mode, const unsigned int count) const
{
	vao.Bind();
	//ibo.Bind();
	glDrawArrays(mode, 0, count);
	//ibo.Unbind();
	vao.Unbind();
}


