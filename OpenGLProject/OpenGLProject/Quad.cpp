#include "Quad.h"

#include <glad/glad.h>



Quad::Quad(const Texture& inTexture, const float inXPosition, const float inYPosition, const float inWidth, const float inHeight):
	texture(inTexture), xPosition(inXPosition), yPosition(inYPosition), width(inWidth), height(inHeight)
{
	Compute();
}

void Quad::Compute()
{
	// Each line to be interpreted as (vCoorX, vCoorY, texCoorX, texCoorY)
	quadCoor.reserve(Quad::VERTICES_COUNT * Quad::QUAD_ELMTS_COUNT);
	quadCoor.insert(quadCoor.end(),
		{
		xPosition,			yPosition + height,		0.0f, 0.0f,
		xPosition,			yPosition,				0.0f, 1.0f,
		xPosition + width,	yPosition,				1.0f, 1.0f,

		xPosition,			yPosition + height,		0.0f, 0.0f,
		xPosition + width,	yPosition,				1.0f, 1.0f,
		xPosition + width,	yPosition + height,		1.0f, 0.0f
		}
	);
}

void Quad::Store(VertexBuffer& vbo)
{
	vbo.InitSubData({ { static_cast<const void*>(quadCoor.data()), Quad::GetSize() } });
}

void Quad::Render(const Renderer& renderer, const VertexArray& vao, const uint32_t textureUnit)
{
	texture.Enable(textureUnit);
	renderer.Draw(vao, GL_TRIANGLES, Quad::VERTICES_COUNT);
	texture.Disable();
}