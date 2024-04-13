#include "Text.h"

#include <freetype/freetype.h>
#include <glad.h>
#include <utility>
#include <vector>

#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



Text::Text()
{
	FT_Library FreeTypeLibrary;
	if (FT_Init_FreeType(&FreeTypeLibrary))
	{
		std::cout << "ERROR::FREETYPE - Could not init FreeType Library" << std::endl;
	}

	// Load font as face object
	FT_Face face;
	if (FT_New_Face(FreeTypeLibrary, "../Fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE - Failed to load font" << std::endl;
	}

	// Set pixel font size to extract from face (render quality)
	FT_Set_Pixel_Sizes(face, 0, 100);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load the first 128 characters of ASCII set
	for (unsigned char charCode = 0; charCode < 128; ++charCode)
	{
		if (FT_Load_Char(face, charCode, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE - Failed to load Glyph" << std::endl;
			continue;
		}

		// No need to specify image path here since it's a glyph
		Texture characterTexture("", GL_TEXTURE_2D, GeometryType::CHARACTER, MapType::NONE);
		characterTexture.LoadGlyph(face, GL_RED);

		// Create object storing current ASCII character caracteristics (glyph non-null, check passed above)
		Character character =
		{
			characterTexture.GetRendererID(),
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		// Store character for later use
		characters.insert(std::make_pair(charCode, character));
	}

	// Destroy FreeType once work is finished
	FT_Done_Face(face);
	FT_Done_FreeType(FreeTypeLibrary);

	Store();
}

Text::~Text()
{
	if (vbo)
	{
		vbo->~VertexBuffer();
	}

	if (vao)
	{
		vao->~VertexArray();
	}
}

void Text::Store()
{
	// Configure VAO/VBO for 2D quads in which we will render character textures
	vao = new VertexArray();
	vbo = new VertexBuffer(nullptr, sizeof(float) * 6 * 4);

	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, GetPositionElmtsCount());
	vao->AddBuffer(*vbo, vbl);

	vbo->Unbind();
	vao->Unbind();
}

float Text::GetBillboardSize(const std::string& text, const float scale)
{
	float totalAdvance = 0.0f;
	for (const auto& c: text)
	{
		totalAdvance += (characters[c].advance >> 6) * scale;
	}

	return totalAdvance;
}

void Text::Render(const Renderer& renderer, const std::string text, float x, const float y, const float scale, const unsigned int& textureUnit)
{
	// Shift billboard to left in order to center it to the concerned celestial body
	x = -GetBillboardSize(text, scale) * 0.5f;

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	vao->Bind();

	for (const auto& c : text)
	{
		const Character character = characters[c];

		// Position of the quad
		const float xpos = x + character.bearing.x * scale;
		const float ypos = y - (character.size.y - character.bearing.y) * scale;

		// Size of the quad
		const float width = character.size.x * scale;
		const float height = character.size.y * scale;

		// Update VBO for each character
		std::vector<float> vertices =
		{
			xpos,			ypos + height,  0.0f, 0.0f,
			xpos,			ypos,			0.0f, 1.0f,
			xpos + width,	ypos,			1.0f, 1.0f,

			xpos,			ypos + height,  0.0f, 0.0f,
			xpos + width,	ypos,			1.0f, 1.0f,
			xpos + width,	ypos + height,  1.0f, 0.0f
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, character.rendererID);

		// Update content of VBO memory
		vbo->Bind();
		vbo->InitSubData({ { static_cast<const void*>(vertices.data()), static_cast<unsigned int>(vertices.size()) * sizeof(float) } });
		vbo->Unbind();

		renderer.Draw(*vao, GL_TRIANGLES, 6);

		// Advance cursors for next glyph (note that advance is number of 1/64 pixels)
		// bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		x += (character.advance >> 6) * scale;
	}

	vao->Unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
}