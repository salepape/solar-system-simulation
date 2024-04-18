#include "Text.h"

#include <freetype/freetype.h>
#include <glad.h>
#include <utility>
#include <vector>

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



Text::Text()
{

}

Text::Text(const int count)
{
	LoadASCIICharacters(count);
	AllocateBufferObjects();
}

Text::~Text()
{
	//if (vao)
	//{
	//	delete vao;
	//}

	//if (vbo)
	//{
	//	delete vbo;
	//}
}

void Text::LoadASCIICharacters(const int count)
{
	FT_Library FreeTypeLibrary;
	if (FT_Init_FreeType(&FreeTypeLibrary))
	{
		std::cout << "ERROR::FREETYPE - Failed to initialise FreeType Library" << std::endl;
	}

	// Load font as face object
	FT_Face face;
	if (FT_New_Face(FreeTypeLibrary, "../Fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE - Failed to load the font" << std::endl;
	}

	// Set pixel font size to extract from face (render quality)
	FT_Set_Pixel_Sizes(face, 0, 100);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char charCode = 0; charCode < count; ++charCode)
	{
		if (FT_Load_Char(face, charCode, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE - Failed to load the face object glyph" << std::endl;
			continue;
		}

		FT_GlyphSlot glyph = face->glyph;
		if (glyph == nullptr)
		{
			std::cout << "ERROR::FREETYPE - Glyph slot is abnormally empty" << std::endl;
			continue;
		}

		// No need to specify an image path here since the glyph bitmap directly contains the data
		Texture glyphTexture("", GL_TEXTURE_2D, { GL_CLAMP_TO_EDGE }, { GL_LINEAR }, TextureType::NONE);
		glyphTexture.LoadFTBitmap(glyph->bitmap, GL_RED);

		// Create object storing current ASCII character glyph caracteristics
		GlyphParams glyphParams(
			glyphTexture,
			glm::ivec2(glyph->bitmap.width, glyph->bitmap.rows),
			glm::ivec2(glyph->bitmap_left, glyph->bitmap_top),
			face->glyph->advance.x
		);

		// Store character for later use
		characters.insert(std::make_pair(charCode, glyphParams));
	}

	// Destroy FreeType once work is finished
	FT_Done_Face(face);
	FT_Done_FreeType(FreeTypeLibrary);
}

void Text::AllocateBufferObjects()
{
	// Configure VAO/VBO for 2D quads in which we will render character textures
	vao = new VertexArray();
	vbo = new VertexBuffer(nullptr, static_cast<size_t>(sizeof(float) * VERTICES_COUNT * QUAD_ELMTS_COUNT));

	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, QUAD_ELMTS_COUNT);
	vao->AddBuffer(*vbo, vbl);

	vao->Unbind();
	vbo->Unbind();
}

float Text::GetBillboardSize(const std::string& text, const float scale)
{
	float totalAdvance = 0.0f;
	for (const auto& c: text)
	{
		totalAdvance += GetGlyphAdvance(characters[c], scale);
	}

	return totalAdvance;
}

float Text::GetGlyphAdvance(const GlyphParams& glyphParams, const float scale) const
{
	// Bitshift by VERTICES_COUNT to get value in pixels (2^VERTICES_COUNT = 64, so we divide 1/64th pixels by 64 to get the amount of pixels))
	return (glyphParams.advance >> VERTICES_COUNT) * scale;
}

void Text::Render(const Renderer& renderer, const std::string text, float x, const float y, const float scale, const unsigned int& textureUnit)
{
	// Shift billboard to left in order to center it to the concerned celestial body
	x = -GetBillboardSize(text, scale) * 0.5f;

	vao->Bind();

	const size_t verticesQuadSize = VERTICES_COUNT * QUAD_ELMTS_COUNT * sizeof(float);

	for (const auto& c : text)
	{
		const GlyphParams& glyphParams = characters[c];

		// Position of the quad
		const float xpos = x + glyphParams.bearing.x * scale;
		const float ypos = y - (glyphParams.size.y - glyphParams.bearing.y) * scale;

		// Size of the quad
		const float width = glyphParams.size.x * scale;
		const float height = glyphParams.size.y * scale;

		// Vector contains VERTICES_COUNT * QUAD_ELMTS_COUNT
		std::vector<float> verticesQuad =
		{
			xpos,			ypos + height,  0.0f, 0.0f,
			xpos,			ypos,			0.0f, 1.0f,
			xpos + width,	ypos,			1.0f, 1.0f,

			xpos,			ypos + height,  0.0f, 0.0f,
			xpos + width,	ypos,			1.0f, 1.0f,
			xpos + width,	ypos + height,  1.0f, 0.0f
		};

		// Update content of VBO memory
		vbo->InitSubData({ { static_cast<const void*>(verticesQuad.data()), verticesQuadSize } });

		// Render glyph texture over quad
		glyphParams.texture.Enable(textureUnit);
		renderer.Draw(*vao, GL_TRIANGLES, VERTICES_COUNT);
		glyphParams.texture.Disable();

		x += GetGlyphAdvance(glyphParams, scale);
	}

	vao->Unbind();
}