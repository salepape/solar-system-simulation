#include "VertexBufferLayout.h"

#include <glad.h>



VertexBufferLayout::VertexBufferLayout()
{

}

VertexBufferLayout::~VertexBufferLayout()
{

}

void VertexBufferLayout::AddAttributeLayout(const VertexAttributeLocation location, const unsigned int type, const unsigned int count)
{
	VertexAttributeLayout attributeLayout;
	attributeLayout.location = location;
	attributeLayout.type = type;
	attributeLayout.count = count;
	attributeLayout.normalized = GL_FALSE;
	attributeLayouts.push_back(attributeLayout);

	stride += static_cast<size_t>(count * sizeof(type));
}