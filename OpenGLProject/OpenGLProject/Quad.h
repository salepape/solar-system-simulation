#ifndef QUAD_H
#define QUAD_H

#include <glm/vec2.hpp>
#include <vector>



class Renderer;
class VertexArray;
class VertexBuffer;



struct QuadVertex
{
	glm::vec2 position{ 0.0f };
	glm::vec2 texCoords{ 0.0f };

	static constexpr uint32_t ELMTS_COUNT = 4;
};

// A 2D quad used to render a texture (e.g. a character glyph). Not inheriting from Mesh class since we do not want the same vertex elements/VAO management.
class Quad
{
public:
	Quad(const float inXPosition, const float inYPosition, const float inWidth, const float inHeight);

	// Update content of VBO memory with quad data
	void StoreVertices(VertexBuffer& vbo);

	// Render texture over the quad (e.g. the one of a character glyph)
	void Render(const Renderer& renderer, const VertexArray& vao);

	static constexpr int32_t VERTICES_COUNT = 6;
	static constexpr size_t GetSize() { return static_cast<size_t>(Quad::VERTICES_COUNT) * sizeof(QuadVertex); }

private:
	float xPosition{ 0.0f };
	float yPosition{ 0.0f };
	float width{ 0.0f };
	float height{ 0.0f };

	std::vector<QuadVertex> vertexCoor;

	void ComputeVertices();
};



#endif // QUAD_H