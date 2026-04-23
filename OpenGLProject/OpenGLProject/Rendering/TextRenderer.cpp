#include "TextRenderer.h"

#include <glad/glad.h>

#include <cassert>
#include <iostream>
#include <utility>

#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Components/Meshes/QuadMeshComponent.h"
#include "Helpers.h"
#include "Renderer.h"



TextRenderer::TextRenderer()
{
	AllocateBufferObjects();

	LoadFreeTypeLibrary();
	LoadFreeTypeFace(FileHelper::GetSolutionAbsolutePath() + "/Fonts/arial.ttf");

	LoadFreeTypeGlyphs();

	ClearFreeTypeResources();
}

void TextRenderer::AllocateBufferObjects()
{
	vao = std::make_shared<VertexArray>();
	vbo = std::make_shared<VertexBuffer>(nullptr, QuadMeshComponent::GetSize(), GL_DYNAMIC_DRAW);

	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, QuadVertex::ELMTS_COUNT);
	vao->AddBuffer(std::move(vbl));

	vao->Unbind();
	vbo->Unbind();
}

void TextRenderer::LoadFreeTypeGlyphs()
{
	// Warning - Any character appearing in rendered text should be listed below
	for (const char& character : "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'")
	{
		const FT_Error FTLoaderCharError = FT_Load_Char(FreeTypeFace, character, FT_LOAD_RENDER);
		if (FTLoaderCharError != 0)
		{
			std::cout << "ERROR::FREETYTPE - Failed to load the face object glyph for character " << character << "!" << std::endl;
			assert(false);
		}

		const FT_GlyphSlot glyph = FreeTypeFace->glyph;
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
		glyphTexture.LoadBitmapImage(glyph->bitmap.width, glyph->bitmap.rows, GL_RED, glyph->bitmap.buffer);

		// Use direct-list-initialisation to avoid having to add constructors in the plain-old data struct
		const GlyphParams glyphParams{
			std::vector<Texture>{ std::move(glyphTexture) },
			glyph->bitmap.width,
			glyph->bitmap.rows,
			glm::ivec2(glyph->bitmap_left, glyph->bitmap_top),
			FreeTypeFace->glyph->advance.x
		};

		ASCIICharacterCache.emplace(character, glyphParams);
	}
}

void TextRenderer::LoadFreeTypeLibrary()
{
	const FT_Error FTInitFreeTypeError = FT_Init_FreeType(&FreeTypeLibrary);
	if (FTInitFreeTypeError != 0)
	{
		std::cout << "ERROR::FREETYPE - Failed to initialise FreeType Library" << std::endl;
		assert(false);
	}
}

void TextRenderer::LoadFreeTypeFace(const std::string& fontPath)
{
	// Load font as face object (to be used for all glyphs that will be loaded further down the pipe)
	const FT_Error FTNewFaceError = FT_New_Face(FreeTypeLibrary, fontPath.c_str(), 0, &FreeTypeFace);
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
	FT_Set_Pixel_Sizes(FreeTypeFace, pixelFontWidth, pixelFontHeight);

	// Disable default OpenGL restriction for each texel to be coded as a multiple of 4 bytes (rgba) due to 
	// each FreeType glyph bitmap colour being coded on 1 byte
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void TextRenderer::ClearFreeTypeResources() const
{
	FT_Done_Face(FreeTypeFace);
	FT_Done_FreeType(FreeTypeLibrary);
}

void TextRenderer::Render(const uint32_t textureUnit, const std::string& text, float x, const float y, const float scale)
{
	// Left-shift billboard position to half its width to center-align it to the celestial body
	x = -GetBillboardSize(text, scale) * 0.5f;

	vao->Bind();

	for (const char& character : text)
	{
		const GlyphParams& glyphParams = ASCIICharacterCache[static_cast<int8_t>(character)];
		if (glyphParams.textures.size() <= 0)
		{
			std::cout << "ERROR::TEXT_RENDERER - No 2D Glyph has been generated for character " << character << " - check if this is an unsupported character in the ASCII table" << std::endl;
			assert(false);
		}

		// Position of the quad on the billboard
		const float xPosition = x + glyphParams.bearing.x * scale;
		const float yPosition = y - (glyphParams.height - glyphParams.bearing.y) * scale;

		// Size of the quad
		const float width = glyphParams.width * scale;
		const float height = glyphParams.height * scale;

		QuadMeshComponent quad(xPosition, yPosition, width, height);
		quad.StoreVertices(*vbo);

		// @todo - Enabling/Disabling operations should be brought outside the for-loop for optimisation num of permutations
		glyphParams.textures[0].Enable(textureUnit);
		quad.Render(*vao);
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
	return (glyphParams.advance >> QuadMeshComponent::VERTICES_COUNT) * scale;
}