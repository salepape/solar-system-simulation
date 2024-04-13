#include "Texture.h"

#include <glad.h>
#include <SOIL2.h>



// @todo - Implement effect of mapType of Texture object
Texture::Texture(const std::string& inPath, const unsigned int InTarget, const GeometryType geometryType, const MapType InMapType) :
	path(inPath), target(InTarget), mapType(InMapType)
{
	switch (geometryType)
	{
	case GeometryType::CIRCLE:
	case GeometryType::SPHERE:
	case GeometryType::MODEL:
	{
		SetWraps(GL_REPEAT);
		SetFilters(GL_LINEAR);
		break;
	}
	case GeometryType::CUBE:
	{
		SetWraps(GL_CLAMP_TO_EDGE);
		SetFilters(GL_LINEAR);
		break;
	}
	case GeometryType::CHARACTER:
	{
		// Generate then bind Texture Object (= TO) to the OpenGL TO type we want 
		glGenTextures(1, &rendererID);
		glBindTexture(target, rendererID);

		SetWraps(GL_CLAMP_TO_EDGE);
		SetFilters(GL_LINEAR);
		break;
	}
	}
}

Texture::~Texture()
{

}

void Texture::LoadTextureImage(const unsigned int channel) const
{
	int width = 0;
	int height = 0;
	int channelsCount = 0;
	
	unsigned char* data = SOIL_load_image(path.c_str(), &width, &height, &channelsCount, channel);
	if (data == nullptr)
	{
		std::cout << "ERROR::SOIL - Failed to load entity sprite at path " << path << std::endl;
		return;
	}

	unsigned int format = GL_RGBA;
	switch (channelsCount)
	{
	case 1:
	{
		format = GL_RED;
		break;
	}
	case 3:
	{
		format = GL_RGB;
		break;
	}
	case 4:
	{
		format = GL_RGBA;
		break;
	}
	}

	Bind();

	// Generate texture image on the currently bound TO at the active texture unit
	glTexImage2D(target, 0, format, width, height, channelsCount, format, GL_UNSIGNED_BYTE, data);

	// Generate all mipmap levels for the previous currently bound TO
	glGenerateMipmap(target);

	SOIL_free_image_data(data);
}

void Texture::LoadGlyph(const FT_Face face, const unsigned int format) const
{
	if (face == false)
	{
		return;
	}

	glTexImage2D(target, 0, format, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, format, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::LoadDDS()
{
	// Already contains glGenTextures function!!!
	rendererID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (rendererID == 0)
	{
		printf("ERROR::SOIL - Loading error: '%s'\n", SOIL_last_result());
	}

	Bind();
}

void Texture::LoadCubemapDDS()
{
	// Already contains glGenTextures function!!!
	rendererID = SOIL_load_OGL_single_cubemap(path.c_str(), "EWUDNS", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (rendererID == 0)
	{
		printf("ERROR::SOIL - Loading error: '%s'\n", SOIL_last_result());
	}

	Bind();
}

void Texture::SetWraps(const unsigned int wrapType) const
{
	if (target == GL_TEXTURE_CUBE_MAP)
	{
		SetWraps(wrapType, wrapType, wrapType);
	}
	else
	{
		SetWraps(wrapType, wrapType);
	}
}

void Texture::SetWraps(const unsigned int s, const unsigned int t) const
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
}

void Texture::SetWraps(const unsigned int s, const unsigned int t, const unsigned int r) const
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, r);
}

void Texture::SetFilters(const unsigned int filterType) const
{
	SetFilters(filterType, filterType);
}

void Texture::SetFilters(const unsigned int min, const unsigned int mag) const
{
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
}

void Texture::Bind() const
{
	glBindTexture(target, rendererID);
}

void Texture::Unbind() const
{
	glBindTexture(target, 0);
}

void Texture::Enable(unsigned int textUnit) const
{
	glActiveTexture(GL_TEXTURE0 + textUnit);
	Bind();
}

void Texture::Disable() const
{
	glActiveTexture(0);
	Unbind();
}