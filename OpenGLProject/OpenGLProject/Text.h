#ifndef TEXT_H
#define TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/vec2.hpp>
#include <iostream>
#include <map>
#include <string>

class Renderer;
class VertexArray;
class VertexBuffer;



// Holds all state information relevant to a glyph as loaded using FreeType (including metrics)
struct GlyphParams
{
	unsigned int rendererID;
	glm::ivec2 size;
	glm::ivec2 bearing;			// Offset from baseline to left/top of glyph
	FT_Pos advance;				// Horizontal offset to advance to start next glyph
};

// Text encompassing all texts (name of each celestial body) appearing within the simulation
class Text
{
public:
	Text(const int inCount);
	~Text();

	// Update VBO for each character of the text provided as input
	void Render(const Renderer& renderer, const std::string text, float x, const float y, const float scale, const unsigned int& textUnit);

private:
	std::map<char, GlyphParams> characters;
	VertexArray* vao{ nullptr };
	VertexBuffer* vbo{ nullptr };

	// Load ASCII characters (data + texture creation), up to the number indicated in input
	void LoadASCIICharacters(const int count);

	// Only allocate VBO and VAO (will be filled with subdata later)
	void AllocateBufferObjects();

	// Return the width of the text (spaces included)
	float GetBillboardSize(const std::string& text, const float scale);

	// Get the advance value (number of 1/64 pixels)
	float GetGlyphAdvance(const GlyphParams& c, const float scale) const;

	static constexpr int VERTICES_COUNT = 6;
	static constexpr int QUAD_ELMTS_COUNT = 4;
};



#endif // TEXT_H