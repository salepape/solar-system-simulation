#include "QuadMeshComponent.h"

#include <glad/glad.h>

#include "Buffers/VertexBuffer.h"
#include "Rendering/Renderer.h"



QuadMeshComponent::QuadMeshComponent(const float inXPosition, const float inYPosition, const float inWidth, const float inHeight) :
	xPosition(inXPosition), yPosition(inYPosition), width(inWidth), height(inHeight)
{
	ComputeVertices();
}

void QuadMeshComponent::ComputeVertices()
{
	vertexCoor.reserve(QuadMeshComponent::VERTICES_COUNT);

	vertexCoor.insert(vertexCoor.end(), {
		{ { xPosition,			yPosition + height }, { 0.0f, 0.0f } },
		{ { xPosition,			yPosition		   }, { 0.0f, 1.0f } },
		{ { xPosition + width,	yPosition		   }, { 1.0f, 1.0f } },

		{ { xPosition,			yPosition + height }, { 0.0f, 0.0f } },
		{ { xPosition + width,	yPosition		   }, { 1.0f, 1.0f } },
		{ { xPosition + width,	yPosition + height }, { 1.0f, 0.0f } }
		});
}

void QuadMeshComponent::StoreVertices(VertexBuffer& vbo) const
{
	vbo.SetSubData(static_cast<const void*>(vertexCoor.data()), QuadMeshComponent::GetSize());
}

void QuadMeshComponent::Render(const VertexArray& vao) const
{
	Renderer::Draw(vao, GL_TRIANGLES, QuadMeshComponent::VERTICES_COUNT);
}