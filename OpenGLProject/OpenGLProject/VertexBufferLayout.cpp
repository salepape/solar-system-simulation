#include "VertexBufferLayout.h"



VertexBufferLayout::VertexBufferLayout() : stride(0)
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

	stride += count * sizeof(type);
}