#ifndef QUAD_H
#define QUAD_H

#include <glm/vec2.hpp>

#include <cstddef> // std::size_t
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class VertexArray;



struct QuadParams
{
	float xPosition{ 0.0f };
	float yPosition{ 0.0f };

	float width{ 0.0f };
	float height{ 0.0f };
};

// Warning: make sure this struct always contain the same data as the mirrored attribute (called 'va_Quad') in the GLSL Vertex Shader
struct Vertex2D
{
	glm::vec2 position{ 0.0f };
	glm::vec2 texCoords{ 0.0f };

	static constexpr uint32_t POSITION_TYPE_DIMENSION = 2;
	static constexpr uint32_t TEXCOORDS_TYPE_DIMENSION = 2;
};

// A chain of 2D Quad Mesh. Not inheriting from Mesh class which is for element layout/VAO management in 3D only
class QuadMeshComponent
{
public:
	QuadMeshComponent(std::vector<QuadParams>&& inQuadParams);

	// Render texture over the quad (e.g. the one of a character glyph)
	void RenderGlyphs(const std::string& text, const uint32_t textureUnit) const;

	// Amount of vertices needed to shape a quad (formed of 2 triangles)
	static constexpr int32_t QUAD_VERTEX_COUNT = 6;

private:
	// Quad coordinates/dimensions computed from another class, needed to build actual Quad vertices
	std::vector<QuadParams> allQuadParams;
	// List of Quad vertices (to be read per batch of 6 elements - batch not represented in data)
	std::vector<Vertex2D> vertices;

	std::shared_ptr<VertexArray> vao;

	void ComputeVertices();

	// Allocate needed VBO memory for quad data and communicate to VAO how to interpret its layout
	void StoreVertices();
};



#endif // QUAD_H