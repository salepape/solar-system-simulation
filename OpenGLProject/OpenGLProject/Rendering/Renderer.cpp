#include "Renderer.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Shader.h"



bool Renderer::IsOpenGLContextActive()
{
	const GLFWwindow* const windowWithCurrentOpenGLContext = glfwGetCurrentContext();
	const bool isOpenGLContextActive = windowWithCurrentOpenGLContext == nullptr;

	if (isOpenGLContextActive == false)
	{
		std::cout << "ERROR::RENDERER - No current OpenGL context running for the GLFW Window: OpenGL functions cannot work!" << std::endl;
		assert(false);
	}

	return isOpenGLContextActive;
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EnableDepthTesting()
{
	glEnable(GL_DEPTH_TEST);
}

void Renderer::EnableFaceCulling()
{
	glEnable(GL_CULL_FACE);
}

void Renderer::DisableFaceCulling()
{
	glDisable(GL_CULL_FACE);
}

void Renderer::EnableBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::DisableDepthTesting()
{
	glDisable(GL_DEPTH_TEST);
}

void Renderer::DisableBlending()
{
	glDisable(GL_BLEND);
}

void Renderer::SetDepthFctToEqual()
{
	glDepthFunc(GL_LEQUAL);
}

void Renderer::SetDepthFctToLess()
{
	glDepthFunc(GL_LESS);
}

void Renderer::SetModelMatrixVUniform(const Shader& shader, const glm::mat4& modelMatrix)
{
	shader.setUniformMat4("vu_Model", modelMatrix);
}

void Renderer::Draw(const VertexArray& vao, const unsigned int mode, const int32_t startIndex, const unsigned int count)
{
	vao.Bind();

	glDrawArrays(mode, startIndex, count);

	vao.Unbind();
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo)
{
	vao.Bind();
	ibo.Bind();

	glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);

	// Do NOT unbind IBO before VAO since the latter contains references to IBO BindBuffer
	vao.Unbind();
	ibo.Unbind();
}

void Renderer::DrawInstances(const VertexArray& vao, const unsigned int iboCount, const unsigned int instanceCount)
{
	vao.Bind();

	glDrawElementsInstanced(GL_TRIANGLES, iboCount, GL_UNSIGNED_INT, 0, instanceCount);

	vao.Unbind();
}