#include "QuadMeshComponent.h"

#include <glad/glad.h>
#include <iostream>
#include <utility>

#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Rendering/Renderer.h"
#include "Rendering/GlyphLoader.h"



QuadMeshComponent::QuadMeshComponent(std::vector<QuadParams>&& inQuadParams) :
	allQuadParams(inQuadParams)
{
	ComputeVertices();
	StoreVertices();
}

void QuadMeshComponent::ComputeVertices()
{
	vertices.reserve(allQuadParams.size() * static_cast<size_t>(QuadMeshComponent::QUAD_VERTEX_COUNT));

	// Compute batch of 6 vertices (forming a quad) to render each glyph letter of the provided text (e.g. on a billboard)
	for (const QuadParams& quadParams : allQuadParams)
	{
		// Lower triangle
		vertices.emplace_back(Vertex2D{ glm::vec2(quadParams.xPosition,						quadParams.yPosition + quadParams.height),	glm::vec2(0.0f, 0.0f) });
		vertices.emplace_back(Vertex2D{ glm::vec2(quadParams.xPosition,						quadParams.yPosition),						glm::vec2(0.0f, 1.0f) });
		vertices.emplace_back(Vertex2D{ glm::vec2(quadParams.xPosition + quadParams.width,	quadParams.yPosition),						glm::vec2(1.0f, 1.0f) });

		// Upper triangle
		vertices.emplace_back(Vertex2D{ glm::vec2(quadParams.xPosition,						quadParams.yPosition + quadParams.height),	glm::vec2(0.0f, 0.0f) });
		vertices.emplace_back(Vertex2D{ glm::vec2(quadParams.xPosition + quadParams.width,	quadParams.yPosition),						glm::vec2(1.0f, 1.0f) });
		vertices.emplace_back(Vertex2D{ glm::vec2(quadParams.xPosition + quadParams.width,	quadParams.yPosition + quadParams.height),	glm::vec2(1.0f, 0.0f) });
	}
}

void QuadMeshComponent::StoreVertices()
{
	if (vertices.empty())
	{
		std::cout << "ERROR::QUAD_MESH - No vertices found!" << std::endl;
		assert(false);
	}

	vao = std::make_shared<VertexArray>();
	VertexBuffer vbo(static_cast<const void*>(vertices.data()), vertices.size() * sizeof(Vertex2D));

	// For 2D character glyphs, GLSL Vertex attribute definition can be more simply defined, i.e. as a single Vec4 instead of 2 Vec2
	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, Vertex2D::POSITION_TYPE_DIMENSION + Vertex2D::TEXCOORDS_TYPE_DIMENSION);
	vao->RegisterVertexBufferLayout(std::move(vbl));

	vao->Unbind();
	vbo.Unbind();
}

void QuadMeshComponent::RenderGlyphs(const std::string& text, const uint32_t textureUnit) const
{
	vao->Bind();

	for (int i = 0; i < text.size(); ++i)
	{
		const char& character = text[i];

		const GlyphParams& glyphParams = GlyphLoader::GetGlyphParams(static_cast<int8_t>(character));
		if (glyphParams.textures.size() <= 0)
		{
			std::cout << "ERROR::QUAD_MESH - No 2D quad has been generated for character glyph " << character << " - check if this is an unsupported one in the ASCII table" << std::endl;
			assert(false);
		}

		glyphParams.textures[0].Enable(textureUnit);
		Renderer::Draw(GL_TRIANGLES, i * QuadMeshComponent::QUAD_VERTEX_COUNT, QuadMeshComponent::QUAD_VERTEX_COUNT);
		glyphParams.textures[0].Disable();
	}

	vao->Unbind();
}