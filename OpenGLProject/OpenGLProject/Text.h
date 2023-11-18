#ifndef TEXT_H
#define TEXT_H

#include <ft2build.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <iostream>
#include <map>
#include <vector>

#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include FT_FREETYPE_H



// Holds all state information relevant to a character as loaded using FreeType (including metrics)
struct Character
{
	unsigned int rendererID;	// ID handle of the glyph texture
	glm::ivec2 Size;			// Size of glyph
	glm::ivec2 Bearing;			// Offset from baseline to left/top of glyph
	FT_Pos Advance;				// Horizontal offset to advance to next glyph
};

// Text encompassing all texts (name of each celestial body) appearing within the simulation
class Text
{
private:
	std::map<GLchar, Character> characters;
	VertexArray * vao;
	VertexBuffer * vbo;

	// Return the width of the text (spaces included)
	float GetBillboardSize(const std::string text, const float scale);

public:
	// Creature texture for each ASCII character
	Text();
	~Text();

	// Render line of text
	void Render(Renderer& renderer, std::string text, float x, float y, float scale, unsigned int& textUnit);
};



#endif // TEXT_H