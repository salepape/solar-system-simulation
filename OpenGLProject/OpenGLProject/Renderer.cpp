#include "Renderer.h"

#include <glad.h>

#include "IndexBuffer.h"
#include "VertexArray.h"



void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DepthTest() const
{
	glEnable(GL_DEPTH_TEST);
}

void Renderer::Blend() const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::DepthEqual() const
{
	glDepthFunc(GL_LEQUAL);
}

void Renderer::DepthLess() const
{
	glDepthFunc(GL_LESS);
}

void Renderer::Draw(const VertexArray& vao, const unsigned int mode, const unsigned int count) const
{
	vao.Bind();

	glDrawArrays(mode, 0, count);

	vao.Unbind();
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo) const
{
	vao.Bind();
	ibo.Bind();

	glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);	

	// Do NOT unbind IBO before VAO since the latter contains references to IBO BindBuffer
	vao.Unbind();
	ibo.Unbind();
}

void Renderer::DrawInstances(const VertexArray& vao, const unsigned int iboCount, const unsigned int instanceCount) const
{
	vao.Bind();

	glDrawElementsInstanced(GL_TRIANGLES, iboCount, GL_UNSIGNED_INT, 0, instanceCount);

	vao.Unbind();
}