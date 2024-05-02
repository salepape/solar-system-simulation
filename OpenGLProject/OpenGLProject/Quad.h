#ifndef QUAD_H
#define QUAD_H

class VertexArray;

#include <vector>

#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"



// A 2D quad used to render a texture (e.g. a character glyph)
class Quad
{
public:
	Quad(const Texture& inTexture, const float inXPosition, const float inYPosition, const float inWidth, const float inHeight);

	// Update content of VBO memory with quad data
	void Store(VertexBuffer& vbo);

	// Render texture over the quad (e.g. the one of a character glyph)
	void Render(const Renderer& renderer, const VertexArray& vao, const uint32_t textureUnit);

	static constexpr int32_t VERTICES_COUNT = 6;
	static constexpr int32_t QUAD_ELMTS_COUNT = 4;
	static constexpr size_t GetSize() { return static_cast<size_t>(Quad::VERTICES_COUNT * Quad::QUAD_ELMTS_COUNT) * sizeof(float); }

private:
	const Texture& texture;
	float xPosition{ 0 };
	float yPosition{ 0 };
	float width{ 0 };
	float height{ 0 };

	void Compute();

	std::vector<float> quadCoor;
};

#endif // QUAD_H