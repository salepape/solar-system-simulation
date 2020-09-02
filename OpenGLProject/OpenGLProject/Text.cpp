#include "Text.h"



// Return the width of the text (spaces included)
float Text::billboardSize(std::string text, float scale)
{
	float totalAdvance = 0.0f;
	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
	{
		totalAdvance += (Characters[*c].Advance >> 6) * scale;
	}

	return totalAdvance;
}

// Creature texture for each ASCII character
Text::Text()
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
	vao = new VertexArray();
	vbo = new VertexBuffer(NULL, sizeof(float) * 6 * 4);

	VertexBufferLayout vbl;
	vbl.Push<float>(4);
	vao->AddBuffer(*vbo, vbl);

	vbo->Unbind();
	vao->Unbind();
}

Text::~Text()
{
	vbo->~VertexBuffer();
	vao->~VertexArray();
}

// Render line of text
void Text::Render(std::string text, float x, float y, float scale, GLuint textUnit)
{
	// Shift billboard to left in order to center it to the concerned celestial body
	x = -billboardSize(text, scale) * 0.5f;

	glActiveTexture(GL_TEXTURE0 + textUnit);
	vao->Bind();

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
		std::vector<float> vertices =
		{
			xpos,     ypos + h,   0.0f, 0.0f ,
			xpos,     ypos,       0.0f, 1.0f ,
			xpos + w, ypos,       1.0f, 1.0f ,

			xpos,     ypos + h,   0.0f, 0.0f ,
			xpos + w, ypos,       1.0f, 1.0f ,
			xpos + w, ypos + h,   1.0f, 0.0f
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		// Update content of VBO memory
		vbo->Bind();
		std::vector<std::vector<float>*> verticesAddresses{ &vertices };
		vbo->InitSubData(verticesAddresses);
		vbo->Unbind();

		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale;											// bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	// Unbind VAOText
	vao->Unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
}
