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



// Holds all state information relevant to a character as loaded using FreeType (including metrics)
struct Character
{
	unsigned int rendererID;	// ID handle of the glyph texture
	glm::ivec2 size;			// Size of glyph
	glm::ivec2 bearing;			// Offset from baseline to left/top of glyph
	FT_Pos advance;				// Horizontal offset to advance to next glyph
};

// Text encompassing all texts (name of each celestial body) appearing within the simulation
class Text
{
public:
	// Creature texture for each ASCII character
	Text();
	~Text();

	void Store();

	// Render line of text
	void Render(const Renderer& renderer, const std::string text, float x, const float y, const float scale, const unsigned int& textUnit);

private:
	std::map<char, Character> characters;
	VertexArray* vao{ nullptr };
	VertexBuffer* vbo{ nullptr };

	// Return the width of the text (spaces included)
	float GetBillboardSize(const std::string& text, const float scale);

	static constexpr int GetPositionElmtsCount() { return 4; }
};



#endif // TEXT_H