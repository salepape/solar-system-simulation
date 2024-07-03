#include "Quad.h"

#include <glad/glad.h>

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"



Quad::Quad(const float inXPosition, const float inYPosition, const float inWidth, const float inHeight) :
	xPosition(inXPosition), yPosition(inYPosition), width(inWidth), height(inHeight)
{
	ComputeVertices();
}

void Quad::ComputeVertices()
{
	vertexCoor.reserve(Quad::VERTICES_COUNT);

	vertexCoor.insert(vertexCoor.end(), {
		{ { xPosition,			yPosition + height }, { 0.0f, 0.0f } },
		{ { xPosition,			yPosition		   }, { 0.0f, 1.0f } },
		{ { xPosition + width,	yPosition		   }, { 1.0f, 1.0f } },

		{ { xPosition,			yPosition + height }, { 0.0f, 0.0f } },
		{ { xPosition + width,	yPosition		   }, { 1.0f, 1.0f } },
		{ { xPosition + width,	yPosition + height }, { 1.0f, 0.0f } }
		});
}

void Quad::StoreVertices(VertexBuffer& vbo)
{
	vbo.SetSubData(static_cast<const void*>(vertexCoor.data()), Quad::GetSize());
}

void Quad::Render(const Renderer& renderer, const VertexArray& vao)
{
	renderer.Draw(vao, GL_TRIANGLES, Quad::VERTICES_COUNT);
}