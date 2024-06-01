#ifndef QUAD_H
#define QUAD_H

#include <vector>

class Renderer;
class VertexArray;
class VertexBuffer;



// A 2D quad used to render a texture (e.g. a character glyph)
class Quad
{
public:
	Quad(const float inXPosition, const float inYPosition, const float inWidth, const float inHeight);

	// Update content of VBO memory with quad data
	void Store(VertexBuffer& vbo);

	// Render texture over the quad (e.g. the one of a character glyph)
	void Render(const Renderer& renderer, const VertexArray& vao);

	static constexpr int32_t VERTICES_COUNT = 6;
	static constexpr int32_t QUAD_ELMTS_COUNT = 4;
	static constexpr size_t GetSize() { return static_cast<size_t>(Quad::VERTICES_COUNT * Quad::QUAD_ELMTS_COUNT) * sizeof(float); }

private:
	float xPosition{ 0.0f };
	float yPosition{ 0.0f };
	float width{ 0.0f };
	float height{ 0.0f };

	void Compute();

	std::vector<float> quadCoor;
};



#endif // QUAD_H