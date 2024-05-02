#include "TextRenderer.h"

#include <freetype/freetype.h>
#include <glad.h>
#include <utility>
#include <vector>

#include "Quad.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



TextRenderer::TextRenderer(const uint32_t pixelWidth, const uint32_t pixelHeight)
{
	AllocateBufferObjects();

	if (FT_Init_FreeType(&FreeTypeLibrary))
	{
		std::cout << "ERROR::FREETYPE - Failed to initialise FreeType Library" << std::endl;
	}

	// Load font as face object
	if (FT_New_Face(FreeTypeLibrary, "../Fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE - Failed to load the font" << std::endl;
	}

	// Set pixel font size to be retrieved from the face (i.e. render quality)
	// @todo - Might consider Signed Distance Field Fonts technique instead of hardcode pixel size values 
	FT_Set_Pixel_Sizes(face, pixelWidth, pixelHeight);

	// Disable default OpenGL restriction for each texel to be coded as a multiple of 4 bytes (rgba) due to 
	// each FreeType glyph bitmap colour being coded on 1 byte
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

TextRenderer::~TextRenderer()
{
	//if (vao)
	//{
	//	delete vao;
	//}

	//if (vbo)
	//{
	//	delete vbo;
	//}
}

void TextRenderer::AllocateBufferObjects()
{
	vao = new VertexArray();
	vbo = new VertexBuffer(nullptr, Quad::GetSize());

	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, Quad::QUAD_ELMTS_COUNT);
	vao->AddBuffer(*vbo, vbl);

	vao->Unbind();
	vbo->Unbind();
}

void TextRenderer::LoadASCIICharacters(const std::string& text)
{
	for (const auto& character: text)
	{
		// Avoid loading a character previously loaded
		if (ASCIICharacterCache.find(character) != ASCIICharacterCache.end())
		{
			continue;
		}

		if (FT_Load_Char(face, character, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE - Failed to load the face object glyph" << std::endl;
			continue;
		}	

		FT_GlyphSlot glyph = face->glyph;
		if (glyph == nullptr)
		{
			std::cout << "ERROR::FREETYPE - Glyph slot is abnormally empty" << std::endl;
			continue;
		}

		// No need to specify an image path here since the glyph bitmap directly contains the data
		// @todo - Store all characters into a single texture atlas/sprite sheet for better performance
		Texture glyphTexture("", GL_TEXTURE_2D, { GL_CLAMP_TO_EDGE }, { GL_LINEAR }, TextureType::NONE);
		
		// FreeType glyph bitmap is a grayscale 8-bit image, each byte representing a colour, so we can just store it in the
		// first value of vec4 colour, i.e. red
		glyphTexture.LoadFTBitmap(glyph->bitmap, GL_RED);

		// Use direct-list-initialisation to avoid having to add constrcutors in the plain-old data struct
		GlyphParams glyphParams{
			glyphTexture,
			glm::ivec2(glyph->bitmap.width, glyph->bitmap.rows),
			glm::ivec2(glyph->bitmap_left, glyph->bitmap_top),
			face->glyph->advance.x
		};

		ASCIICharacterCache.insert(std::make_pair(character, glyphParams));
	}
}

void TextRenderer::FreeFTResources() const
{
	FT_Done_Face(face);
	FT_Done_FreeType(FreeTypeLibrary);
}

void TextRenderer::Render(const Renderer& renderer, const std::string& text, float x, const float y, const float scale, const uint32_t textureUnit)
{
	// Left-shift billboard position to half its width to center-align it to the celestial body
	x = -GetBillboardSize(text, scale) * 0.5f;

	vao->Bind();

	for (const auto& character : text)
	{
		const GlyphParams& glyphParams = ASCIICharacterCache[character];

		// Position of the quad on the billboard
		const float xPosition = x + glyphParams.bearing.x * scale;
		const float yPosition = y - (glyphParams.size.y - glyphParams.bearing.y) * scale;

		// Size of the quad
		const float width = glyphParams.size.x * scale;
		const float height = glyphParams.size.y * scale;

		Quad quad(glyphParams.texture, xPosition, yPosition, width, height);
		quad.Store(*vbo);
		quad.Render(renderer, *vao, textureUnit);

		x += GetGlyphAdvance(glyphParams, scale);
	}

	vao->Unbind();
}

float TextRenderer::GetBillboardSize(const std::string& text, const float scale)
{
	float totalAdvance = 0.0f;
	for (const auto& character : text)
	{
		totalAdvance += GetGlyphAdvance(ASCIICharacterCache[character], scale);
	}

	return totalAdvance;
}

float TextRenderer::GetGlyphAdvance(const GlyphParams& glyphParams, const float scale) const
{
	// Bitshift by VERTICES_COUNT to get value in pixels (2^VERTICES_COUNT = 64, so we divide 1/64th pixels by 64 to get the amount of pixels)
	return (glyphParams.advance >> Quad::VERTICES_COUNT) * scale;
}