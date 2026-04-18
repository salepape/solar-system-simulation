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

class VertexArray;
class VertexBuffer;



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
	TextRenderer();

	// Update VBO for each character of the text provided as input
	void Render(const uint32_t textureUnit, const std::string& text, float x, const float y, const float scale);

private:
	std::shared_ptr<VertexArray> vao;
	std::shared_ptr<VertexBuffer> vbo;

	FT_Library FreeTypeLibrary;
	FT_Face FreeTypeFace;

	std::unordered_map<int8_t, GlyphParams> ASCIICharacterCache;

	// Only allocate VBO and VAO (will be filled with subdata later) 
	void AllocateBufferObjects();

	// Set up FreeType environment to load 2D glyphs
	void LoadFreeTypeLibrary();
	void LoadFreeTypeFace(const std::string& fontPath);

	// Load ASCII characters of the alphabet + some symbols (e.g. ')
	void LoadFreeTypeGlyphs();

	// Free FreeType resources once we don't have any more letters to load
	void ClearFreeTypeResources() const;

	// Return the width of the text (spaces included)
	float GetBillboardSize(const std::string& text, const float scale);

	// Get the advance value (number of 1/64 pixels)
	float GetGlyphAdvance(const GlyphParams& c, const float scale) const;
};



#endif // TEXT_RENDERER_H