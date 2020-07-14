#include <ft2build.h>
#include FT_FREETYPE_H

#include "Texture.h"

#include <map>
#include <glad/glad.h>
#include <iostream>
#include <glm/vec2.hpp>




///////////////////////// CHARACTERS /////////////////////////

// Holds all state information relevant to a character as loaded using FreeType (including metrics)
struct Character
{
	unsigned int TextureID;		// ID handle of the glyph texture
	glm::ivec2 Size;			// Size of glyph
	glm::ivec2 Bearing;			// Offset from baseline to left/top of glyph
	FT_Pos Advance;				// Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int VBOText, VAOText;



// Creature texture for each ASCII character
void CreateTextTextures()
{
	// FreeType library instance
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face object
	FT_Face face;
	if (FT_New_Face(ft, "../Fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set pixel font size to extract from face (render quality)
	FT_Set_Pixel_Sizes(face, 0, 100);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	Texture * textCharacter = nullptr;

	// Load the first 128 characters of ASCII set
	for (unsigned char c = 0; c < 128; ++c)
	{
		// Load current character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		textCharacter = new Texture("", "", GL_TEXTURE_2D, 1);
		textCharacter->loadGlyph(face, GL_RED);

		// Create object storing current ASCII character caracteristics
		Character character =
		{
			textCharacter->textID,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		// Store character for later use
		Characters.insert(std::pair<char, Character>(c, character));
	}

	// Unbind character textures
	textCharacter->unbind();

	// Destroy FreeType once work is finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure VAO/VBO for 2D quads in which we will render character textures
	glGenVertexArrays(1, &VAOText);
	glGenBuffers(1, &VBOText);
	glBindVertexArray(VAOText);
	glBindBuffer(GL_ARRAY_BUFFER, VBOText);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Return the width of the text (spaces included)
float billboardSize(std::string text, float scale)
{
	float totalAdvance = 0.0f;
	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
	{
		totalAdvance += (Characters[*c].Advance >> 6) * scale;
	}

	return totalAdvance;
}

// Render line of text
void RenderText(std::string text, float x, float y, float scale, GLuint textUnit)
{
	// Shift billboard to left in order to center it to the concerned celestial body
	x = -billboardSize(text, scale) * 0.5f;

	glActiveTexture(GL_TEXTURE0 + textUnit);
	glBindVertexArray(VAOText);

	// Iterate through all characters
	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
	{
		Character ch = Characters[*c];

		// Origin position of the quad
		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		// Size of the quad
		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// Update VBO for each character
		float vertices[6][4] =
		{
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBOText);
		// Be sure to use glBufferSubData and not glBufferData
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		// Unbind VBOText
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale;			// bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	// Unbind VAOText
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}