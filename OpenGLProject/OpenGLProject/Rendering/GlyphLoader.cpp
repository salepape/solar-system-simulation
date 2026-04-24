#include "GlyphLoader.h"

#include <glad/glad.h>

#include <cassert>
#include <iostream>
#include <utility>

#include "Components/Meshes/QuadMeshComponent.h"
#include "Helpers.h"

std::unordered_map<int8_t, GlyphParams> GlyphLoader::ASCIICharacterCache;

std::string GlyphLoader::partialASCIITable = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'";



void GlyphLoader::LoadASCIICharacters()
{
	FT_Library FreeTypeLibrary;
	LoadFreeTypeLibrary(&FreeTypeLibrary);

	FT_Face FreeTypeFontFace;
	LoadFreeTypeFontFace(&FreeTypeFontFace, FreeTypeLibrary, FileHelper::GetSolutionAbsolutePath() + "/Fonts/arial.ttf");

	LoadFreeTypeGlyphs(FreeTypeFontFace);

	ClearFreeTypeResources(FreeTypeLibrary, FreeTypeFontFace);
}

void GlyphLoader::LoadFreeTypeGlyphs(const FT_Face& inFreeTypeFontFace)
{
	// Warning - Any character appearing in rendered text should be listed below
	for (const char& character : partialASCIITable)
	{
		const FT_Error FTLoaderCharError = FT_Load_Char(inFreeTypeFontFace, character, FT_LOAD_RENDER);
		if (FTLoaderCharError != 0)
		{
			std::cout << "ERROR::FREETYTPE - Failed to load the face object glyph for character " << character << "!" << std::endl;
			assert(false);
		}

		const FT_GlyphSlot glyph = inFreeTypeFontFace->glyph;
		if (glyph == nullptr)
		{
			std::cout << "ERROR::FREETYPE - Glyph slot for character " << character << " is abnormally empty!" << std::endl;
			assert(false);
		}

		// No need to specify an image path here since the glyph bitmap loaded below contains the data
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
			inFreeTypeFontFace->glyph->advance.x
		};

		ASCIICharacterCache.emplace(character, glyphParams);
	}
}

void GlyphLoader::LoadFreeTypeLibrary(FT_Library* outFreeTypeLibrary)
{
	const FT_Error FTInitFreeTypeError = FT_Init_FreeType(outFreeTypeLibrary);
	if (FTInitFreeTypeError != 0)
	{
		std::cout << "ERROR::FREETYPE - Failed to initialise FreeType Library" << std::endl;
		assert(false);
	}
}

void GlyphLoader::LoadFreeTypeFontFace(FT_Face* outFreeTypeFontFace, const FT_Library& inFreeTypeLibrary, const std::string& inFontPath)
{
	// Load font as face object (to be used for all glyphs that will be loaded further down the pipe)
	const FT_Error FTNewFaceError = FT_New_Face(inFreeTypeLibrary, inFontPath.c_str(), 0, outFreeTypeFontFace);
	if (FTNewFaceError != 0)
	{
		std::cout << "ERROR::FREETYPE - Failed to load the font located at " << inFontPath << "!" << std::endl;
		assert(false);
	}

	// Put such default parameters to let the FT_Face dynamically compute the width based on the non-null height
	constexpr uint32_t pixelFontWidth = 0;
	constexpr uint32_t pixelFontHeight = 100;

	// Set pixel font size (i.e. render quality) we want to retrieve from this face object
	// @todo - Might consider Signed Distance Field Fonts technique instead of hardcode pixel size values 
	const FT_Error FTSetPixelSizingError = FT_Set_Pixel_Sizes(*outFreeTypeFontFace, pixelFontWidth, pixelFontHeight);
	if (FTSetPixelSizingError != 0)
	{
		std::cout << "ERROR::FREETYPE - Failed to set the size of pixels for the font located at " << inFontPath << "!" << std::endl;
		assert(false);
	}

	// Disable default OpenGL restriction for each texel to be coded as a multiple of 4 bytes (rgba) due to 
	// each FreeType glyph bitmap colour being coded on 1 byte
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void GlyphLoader::ClearFreeTypeResources(const FT_Library& inFreeTypeLibrary, const FT_Face& inFreeTypeFontFace)
{
	const FT_Error FTReleaseFace = FT_Done_Face(inFreeTypeFontFace);
	if (FTReleaseFace != 0)
	{
		std::cout << "ERROR::FREETYPE - Failed to clear resources of the loaded Face." << std::endl;
		assert(false);
	}

	const FT_Error FTReleaseLibrary = FT_Done_FreeType(inFreeTypeLibrary);
	if (FTReleaseLibrary != 0)
	{
		std::cout << "ERROR::FREETYPE - Failed to clear resources of the loaded Library" << std::endl;
		assert(false);
	}
}

GlyphParams& GlyphLoader::GetGlyphParams(const int8_t character)
{
	if (ASCIICharacterCache.find(character) == ASCIICharacterCache.end())
	{
		std::cout << "ERROR::FREETYPE - Cannot access ASCII character at index " << character << " because map length is smaller!" << std::endl;
		assert(false);
	}

	return ASCIICharacterCache[character];
}