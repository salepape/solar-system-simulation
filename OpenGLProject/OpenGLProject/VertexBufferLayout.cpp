#include "VertexBufferLayout.h"

#include <glad.h>



void VertexBufferLayout::AddAttributeLayout(const VertexAttributeLocation location, const uint32_t type, const uint32_t count)
{
	VertexAttributeLayout attributeLayout;
	attributeLayout.location = location;
	attributeLayout.type = type;
	attributeLayout.count = count;
	attributeLayout.normalized = GL_FALSE;
	attributeLayouts.push_back(attributeLayout);

	stride += static_cast<size_t>(count * sizeof(type));
}