#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

#include <map>
#include <vector>
#include <glad/glad.h>
#include <iostream>
#include <glm/vec2.hpp>



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
	float GetBillboardSize(std::string text, float scale);

public:
	// Creature texture for each ASCII character
	Text();
	~Text();

	// Render line of text
	void Render(std::string text, float x, float y, float scale, unsigned int textUnit);
};
