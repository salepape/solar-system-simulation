#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/vec2.hpp>
#include <memory>
#include <unordered_map>
#include <string>

#include "Texture.h"

class Renderer;
class VertexArray;
class VertexBuffer;



// Hold all texture/metrics information relevant to a loaded ASCII character glyph (using FreeType)
struct GlyphParams
{
	Texture texture;

	float width = 0.0f;
	float height = 0.0f;

	// Offset from baseline to left/top of glyph
	glm::ivec2 bearing{ 0, 0 };

	// Horizontal offset to advance to start next glyph
	FT_Pos advance;
};

// Build a Texture instance for each needed ASCII character, and using them to render celestial body names
class TextRenderer
{
public:
	// Put such default parameters to let the FT_Face dynamically compute the width based on the non-null height
	TextRenderer(const Renderer& inRenderer, const std::string& inFontPath, const uint32_t pixelFontWidth = 0, const uint32_t pixelFontHeight = 100);
	TextRenderer(const TextRenderer&) = delete;
	~TextRenderer() = default;

	// Load ASCII characters (data + texture creation) of the text provided as input
	void LoadASCIICharacters(const std::string& text);
	void FreeFTResources() const;

	// Update VBO for each character of the text provided as input
	void Render(const std::string& text, float x, const float y, const float scale);

private:
	static TextRenderer* instance;

	const Renderer& renderer;

	FT_Library FreeTypeLibrary;
	FT_Face face;

	std::shared_ptr<VertexArray> vao;
	std::shared_ptr<VertexBuffer> vbo;

	std::unordered_map<int8_t, GlyphParams> ASCIICharacterCache;

	// Only allocate VBO and VAO (will be filled with subdata later) 
	void AllocateBufferObjects();

	// Return the width of the text (spaces included)
	float GetBillboardSize(const std::string& text, const float scale);

	// Get the advance value (number of 1/64 pixels)
	float GetGlyphAdvance(const GlyphParams& c, const float scale) const;
};



#endif // TEXT_RENDERER_H