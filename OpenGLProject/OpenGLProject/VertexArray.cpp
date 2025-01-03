#include "VertexArray.h"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



VertexArray::VertexArray()
{
	// Reserve an ID available to be used by the VAO as a binding point
	glGenVertexArrays(1, &rendererID);

	Bind();
}

VertexArray::~VertexArray()
{
	Unbind();

	// Delete the VAO and free up the ID that was being used by it
	glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::Bind() const
{
	// Bind to the VAO ID, which will store all subsequent VBO calls until unbound
	glBindVertexArray(rendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	size_t offset = 0;

	for (const VertexAttributeLayout& attributeLayout : layout.GetAttributeLayouts())
	{
		// Enable the attribute index at location i in the vertex shader to be used
		glEnableVertexAttribArray(attributeLayout.location);

		// Store in the VAO how we want OpenGL to interpret the VBO data relative to the attribute index (layout)
		glVertexAttribPointer(attributeLayout.location, attributeLayout.count, attributeLayout.type, attributeLayout.normalised, layout.GetStride(), reinterpret_cast<const void*>(offset));

		offset += static_cast<size_t>(attributeLayout.count) * sizeof(attributeLayout.type);
	}
}

void VertexArray::AddInstancedBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	Bind();

	const size_t mat4SizeInBytes = sizeof(glm::mat4);
	const size_t vec4sizeInBytes = sizeof(glm::vec4);
	size_t offset = 0;

	// Iterate through the instanced matrix only
	for (const VertexAttributeLayout& attributeLayout : layout.GetAttributeLayouts())
	{
		// Enable the attribute index at location i in the vertex shader to be used
		glEnableVertexAttribArray(attributeLayout.location);

		// Store in the currently bound VBO how we want OpenGL to interpret the data corresponding to the 4x4 matrix
		glVertexAttribPointer(attributeLayout.location, attributeLayout.count, attributeLayout.type, attributeLayout.normalised, mat4SizeInBytes, reinterpret_cast<const void*>(offset));

		// Set the rate at which the attribute index advance when rendering multiple instances of primitives in a single draw call
		glVertexAttribDivisor(attributeLayout.location, 1);

		offset += vec4sizeInBytes;
	}

	Unbind();
}