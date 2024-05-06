#include "Renderer.h"

#include <glad.h>

#include "IndexBuffer.h"
#include "VertexArray.h"



void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EnableDepthTesting() const
{
	glEnable(GL_DEPTH_TEST);
}

void Renderer::EnableFaceCulling() const
{
	glEnable(GL_CULL_FACE);
}

void Renderer::DisableFaceCulling() const
{
	glDisable(GL_CULL_FACE);
}

void Renderer::EnableBlending() const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::DisableDepthTesting() const
{
	glDisable(GL_DEPTH_TEST);
}

void Renderer::DisableBlending() const
{
	glDisable(GL_BLEND);
}

void Renderer::SetDepthFctToEqual() const
{
	glDepthFunc(GL_LEQUAL);
}

void Renderer::SetDepthFctToLess() const
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