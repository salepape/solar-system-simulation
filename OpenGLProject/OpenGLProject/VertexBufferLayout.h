#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

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

	unsigned int type;
	unsigned int count;
	unsigned char normalized;
};

class VertexBufferLayout
{
public:
	VertexBufferLayout();
	~VertexBufferLayout();

	void AddAttributeLayout(const VertexAttributeLocation index, const unsigned int type, const unsigned int count);

	inline const std::vector<VertexAttributeLayout> GetAttributeLayouts() const { return attributeLayouts; }

	inline unsigned int GetStride() const { return stride; }

private:
	std::vector<VertexAttributeLayout> attributeLayouts;
	unsigned int stride;
};



#endif // VERTEXBUFFERLAYOUT_H