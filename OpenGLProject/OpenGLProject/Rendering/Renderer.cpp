#include "Renderer.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

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
	shader.SetUniformMat4("vu_Model", modelMatrix);
}

void Renderer::Draw(const unsigned int mode, const int32_t startIndex, const int32_t count)
{
	glDrawArrays(mode, startIndex, count);
}

void Renderer::Draw(const unsigned int mode, const int32_t count, const void* offsetInBytes)
{
	glDrawElements(mode, count, GL_UNSIGNED_INT, offsetInBytes);
}

void Renderer::DrawInstances(const unsigned int mode, const int32_t startIndex, const int32_t count, const int32_t instanceCount)
{
	glDrawArraysInstanced(mode, startIndex, count, instanceCount);
}