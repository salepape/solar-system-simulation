#include "VertexBufferLayout.h"

#include <glad.h>



void VertexBufferLayout::AddAttributeLayout(const VertexAttributeLocation location, const uint32_t type, const uint32_t count)
{
	VertexAttributeLayout attributeLayout;
	attributeLayout.location = location;
	attributeLayout.type = type;
	attributeLayout.count = count;
	attributeLayout.normalised = false;
	attributeLayouts.push_back(attributeLayout);

	stride += count * static_cast<uint32_t>(sizeof(type));
}