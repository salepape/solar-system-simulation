#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/ext/vector_int2.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Texture.h"



// Hold all texture/metrics information relevant to a loaded ASCII character glyph (using FreeType)
struct GlyphParams
{
	// Using a vector mainly for struct construction convenience
	std::vector<Texture> textures;

	unsigned int width{ 0 };
	unsigned int height{ 0 };

	// Offset from baseline to left/top of glyph
	glm::ivec2 bearing{ 0, 0 };

	// Horizontal offset to advance to start next glyph
	FT_Pos advance;
};

// Singleton class allowing a global access point for a single Application class.
class TextRenderer
{
public:
	static void LoadASCIICharacters();

	static GlyphParams& GetGlyphParams(const int8_t index);

private:
	// Hold texture and glyph data per character of the ASCII alphabet. Will be used by all billboard legends
	static std::unordered_map<int8_t, GlyphParams> ASCIICharacterCache;

	// Set up FreeType environment to load 2D glyphs
	static void LoadFreeTypeLibrary(FT_Library* outFreeTypeLibrary);
	static void LoadFreeTypeFontFace(FT_Face* outFreeTypeFontFace, const FT_Library& inFreeTypeLibrary, const std::string& infontPath);

	// Load ASCII characters of the alphabet + some symbols (e.g. ')
	static void LoadFreeTypeGlyphs(const FT_Face& inFreeTypeFontFace);

	// Free FreeType resources once we don't have any more letters to load
	static void ClearFreeTypeResources(const FT_Library& inFreeTypeLibrary, const FT_Face& inFreeTypeFontFace);
};



#endif // TEXT_RENDERER_H