#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <iostream>
#include <vector>



// List of locations of attributes defined as layouts in a vertex shader (we assume this convention for all shader files)
enum VertexAttributeLocation
{
	Position = 0,
	Normal = 1,
	TextCoord = 2,
	Tangent = 3,
	Bitangent = 4,

	InstancedMatrixCol1 = 5,
	InstancedMatrixCol2 = 6,
	InstancedMatrixCol3 = 7,
	InstancedMatrixCol4 = 8,
};

struct VertexAttributeLayout
{
	VertexAttributeLocation location;

	uint32_t type{ 0 };
	uint32_t count{ 0 };
	uint8_t normalised{ 0 };
};

class VertexBufferLayout
{
public:
	void AddAttributeLayout(const VertexAttributeLocation index, const uint32_t type, const uint32_t count);

	const std::vector<VertexAttributeLayout>& GetAttributeLayouts() const { return attributeLayouts; }

	size_t GetStride() const { return stride; }

private:
	std::vector<VertexAttributeLayout> attributeLayouts;
	size_t stride{ 0 };
};



#endif // VERTEXBUFFERLAYOUT_H