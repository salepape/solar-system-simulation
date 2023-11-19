#include "VertexArray.h"



VertexArray::VertexArray()
{
	// GENERATION VAO - Generate 1 VAO 
	glGenVertexArrays(1, &rendererID);
}

VertexArray::~VertexArray()
{
	// CLEAN VAO - Free the resource once it've outlived its purpose
	glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	Bind();
	vbo.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& vbe = elements[i];

		// PERMISSION - Enable the vertex attribute provided its location
		glEnableVertexAttribArray(i);

		// INTERPRETATION - Store in the currently bound VBO how OpenGL should interpret the vertex buffer data
		glVertexAttribPointer(i, vbe.count, vbe.type, vbe.normalized, layout.GetStride(), reinterpret_cast<const GLvoid*>(offset));

		offset += vbe.count * VertexBufferElement::GetTypeSize(vbe.type);
	}
}

// @todo - Can probably be removed
void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout, const std::vector<unsigned int> sizeofs)
{
	Bind();
	vbo.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& vbe = elements[i];

		// PERMISSION - Enable the vertex attribute provided its location
		glEnableVertexAttribArray(i);

		// INTERPRETATION - Store in the currently bound VBO how OpenGL should interpret the vertex buffer data
		glVertexAttribPointer(i, vbe.count, vbe.type, vbe.normalized, vbe.count * VertexBufferElement::GetTypeSize(vbe.type), reinterpret_cast<const GLvoid*>(offset));

		offset += sizeofs[i];
	}
}

void VertexArray::AddInstancedBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	// Instanced Matrix (location = 3 in AsteroidShader.vs, hence i = 3)
	for (unsigned int i = 3; i < 7; ++i)
	{
		const auto& vbe = elements[i-3];

		// PERMISSION - Enable the vertex attribute provided its location
		glEnableVertexAttribArray(i);

		// INTERPRETATION - Store in the currently bound VBO how OpenGL should interpret the vertex buffer data
		glVertexAttribPointer(i, vbe.count, vbe.type, vbe.normalized, sizeof(glm::mat4), reinterpret_cast<const GLvoid*>(offset));
		glVertexAttribDivisor(i, 1);

		offset += sizeof(glm::vec4);
	}
}

void VertexArray::Bind() const
{
	// BINDING VAO - Bind VAO, which thus will store all following VBO calls
	glBindVertexArray(rendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}