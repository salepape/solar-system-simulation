#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include <cstdint>
#include <vector>



// Index at which a GLSL Vertex Shader attribute will be set (we assume this convention for all shader files)
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

// Define a GLSL Vertex Shader attribute (via the 'layout' keyword) at a specific index as well as information on the nature of the variable itself
struct VertexAttributeLayout
{
	VertexAttributeLocation location;

	uint32_t type{ 0 };
	uint32_t count{ 0 };
	uint8_t normalised{ 0 };
};

// Allow to define a set of GLSL attributes according to their corresponding input positions in the GLSL Vertex Shader (set by convention)
class VertexBufferLayout
{
public:
	void AddAttributeLayout(const VertexAttributeLocation& index, const uint32_t type, const uint32_t count);

	const std::vector<VertexAttributeLayout>& GetAttributeLayouts() const { return attributeLayouts; }

	uint32_t GetStride() const { return stride; }

private:
	std::vector<VertexAttributeLayout> attributeLayouts;

	// Note we are not storing the stride as a std::size_t since the OpenGL function needing this parameter requires a GLsizei, i.e. an int
	uint32_t stride{ 0 };
};



#endif // VERTEX_BUFFER_LAYOUT_H