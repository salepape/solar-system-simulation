#include "TextRenderer.h"

#include <cassert>
#include <freetype/freetype.h>
#include <glad.h>
#include <iostream>
#include <utility>

#include "BodySystem.h"
#include "Quad.h"
#include "Renderer.h"
#include "ShaderLoader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



TextRenderer::TextRenderer()
{
	AllocateBufferObjects();

	FT_Error FTInitFreeTypeError = FT_Init_FreeType(&FreeTypeLibrary);
	if (FTInitFreeTypeError != 0)
	{
		std::cout << "ERROR::FREETYPE - Failed to initialise FreeType Library" << std::endl;
		assert(false);
	}
}

void TextRenderer::AllocateBufferObjects()
{
	vao = std::make_shared<VertexArray>();
	vbo = std::make_shared<VertexBuffer>(nullptr, Quad::GetSize(), GL_DYNAMIC_DRAW);

	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, QuadVertex::ELMTS_COUNT);
	vao->AddBuffer(std::move(vbl));

	vao->Unbind();
	vbo->Unbind();
}

void TextRenderer::LoadFTGlyphs(const std::string& text)
{
	for (const char& character : text)
	{
		// Avoid loading a character previously loaded
		if (ASCIICharacterCache.find(character) != ASCIICharacterCache.end())
		{
			continue;
		}

		FT_Error FTLoaderCharError = FT_Load_Char(face, character, FT_LOAD_RENDER);
		if (FTLoaderCharError != 0)
		{
			std::cout << "ERROR::FREETYTPE - Failed to load the face object glyph for character " << character << "!" << std::endl;
			assert(false);
		}

		FT_GlyphSlot glyph = face->glyph;
		if (glyph == nullptr)
		{
			std::cout << "ERROR::FREETYPE - Glyph slot for character " << character << " is abnormally empty!" << std::endl;
			assert(false);
		}

		// No need to specify an image path here since the glyph bitmap directly contains the data
		// @todo - Store all characters into a single texture atlas/sprite sheet for better performance
		Texture glyphTexture("", GL_TEXTURE_2D, { GL_CLAMP_TO_EDGE }, { GL_LINEAR }, TextureType::Enum::DIFFUSE);

		// FreeType glyph bitmaps are 8-bit grayscale images: it means only one channel (black/white) will be used, encoded with 8 bits,
		// so we just need to store the colour result in the first vector component, equivalent to red for RGBA
		glyphTexture.LoadFTBitmap(glyph->bitmap, GL_RED);

		// Use direct-list-initialisation to avoid having to add constructors in the plain-old data struct
		GlyphParams glyphParams{
			{ std::move(glyphTexture) },
			glyph->bitmap.width,
			glyph->bitmap.rows,
			glm::ivec2(glyph->bitmap_left, glyph->bitmap_top),
			face->glyph->advance.x
		};

		ASCIICharacterCache.insert(std::make_pair(character, glyphParams));
	}
}

void TextRenderer::SetFTFont(const std::string& fontPath)
{
	// Load font as face object
	FT_Error FTNewFaceError = FT_New_Face(FreeTypeLibrary, fontPath.c_str(), 0, &face);
	if (FTNewFaceError != 0)
	{
		std::cout << "ERROR::FREETYPE - Failed to load the font located at " << fontPath << "!" << std::endl;
		assert(false);
	}

	// Put such default parameters to let the FT_Face dynamically compute the width based on the non-null height
	constexpr uint32_t pixelFontWidth = 0;
	constexpr uint32_t pixelFontHeight = 100;

	// Set pixel font size (i.e. render quality) we want to retrieve from this face object
	// @todo - Might consider Signed Distance Field Fonts technique instead of hardcode pixel size values 
	FT_Set_Pixel_Sizes(face, pixelFontWidth, pixelFontHeight);

	// Disable default OpenGL restriction for each texel to be coded as a multiple of 4 bytes (rgba) due to 
	// each FreeType glyph bitmap colour being coded on 1 byte
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void TextRenderer::FreeFTResources() const
{
	FT_Done_Face(face);
	FT_Done_FreeType(FreeTypeLibrary);
}

void TextRenderer::Render(const Renderer& renderer, const uint32_t textureUnit, const std::string& text, float x, const float y, const float scale)
{
	// Left-shift billboard position to half its width to center-align it to the celestial body
	x = -GetBillboardSize(text, scale) * 0.5f;

	vao->Bind();

	for (const char& character : text)
	{
		const GlyphParams& glyphParams = ASCIICharacterCache[static_cast<int8_t>(character)];
		if (glyphParams.textures.size() <= 0)
		{
			std::cout << "ERROR::TEXT_RENDERER - Reference to Glyph Texture2D for character " << character << " has been lost!" << std::endl;
			assert(false);
		}

		// Position of the quad on the billboard
		const float xPosition = x + glyphParams.bearing.x * scale;
		const float yPosition = y - (glyphParams.height - glyphParams.bearing.y) * scale;

		// Size of the quad
		const float width = glyphParams.width * scale;
		const float height = glyphParams.height * scale;

		Quad quad(xPosition, yPosition, width, height);
		quad.StoreVertices(*vbo);

		// @todo - Enabling/Disabling operations should be brought outside the for-loop for optimisation num of permutations
		glyphParams.textures[0].Enable(textureUnit);
		quad.Render(renderer, *vao);
		glyphParams.textures[0].Disable();

		x += GetGlyphAdvance(glyphParams, scale);
	}

	vao->Unbind();
}

float TextRenderer::GetBillboardSize(const std::string& text, const float scale)
{
	float totalAdvance = 0.0f;
	for (const char& character : text)
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