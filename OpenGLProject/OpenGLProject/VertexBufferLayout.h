#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <vector>



// @todo - Move this code inside VertexBuffer class?
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
	void AddAttributeLayout(const VertexAttributeLocation& index, const uint32_t type, const uint32_t count);

	const std::vector<VertexAttributeLayout>& GetAttributeLayouts() const { return attributeLayouts; }

	uint32_t GetStride() const { return stride; }

private:
	std::vector<VertexAttributeLayout> attributeLayouts;

	// Note we are not storing the stride as a size_t since the OpenGL function needing this parameter requires a GLsizei, i.e. an int
	uint32_t stride{ 0 };
};



#endif // VERTEXBUFFERLAYOUT_H