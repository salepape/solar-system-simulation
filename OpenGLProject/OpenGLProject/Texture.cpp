#include "Texture.h"

#include <glad.h>
#include <SOIL2.h>



Texture::Texture(const std::string& inPath, const unsigned int inTarget, const unsigned int wrapOption, const unsigned int filterOption, const TextureType inTextureType) :
	path(inPath), target(inTarget), textureType(inTextureType)
{
	SetWraps(wrapOption);
	SetFilters(filterOption);
}

Texture::~Texture()
{

}

void Texture::LoadFTBitmap(const FT_Bitmap bitmap, const unsigned int format)
{
	// Generate then bind Texture Object (= TO) to the OpenGL TO type we want 
	glGenTextures(1, &rendererID);

	Bind();

	// Generate texture image on the currently bound TO at the active texture unit
	glTexImage2D(target, 0, format, bitmap.width, bitmap.rows, 0, format, GL_UNSIGNED_BYTE, bitmap.buffer);
	
	// Generate all mipmap levels for the previous currently bound TO
	glGenerateMipmap(target);

	Unbind();
}

void Texture::LoadDDS()
{
	// Already contains glGenTextures function!!!
	rendererID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (rendererID == 0)
	{
		printf("ERROR::SOIL - Loading error: '%s'\n", SOIL_last_result());
	}
}

void Texture::LoadCubemapDDS()
{
	// Already contains glGenTextures function!!!
	rendererID = SOIL_load_OGL_single_cubemap(path.c_str(), "EWUDNS", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (rendererID == 0)
	{
		printf("ERROR::SOIL - Loading error: '%s'\n", SOIL_last_result());
	}
}

void Texture::SetWraps(const unsigned int wrapOption) const
{
	if (target == GL_TEXTURE_CUBE_MAP)
	{
		SetWraps(wrapOption, wrapOption, wrapOption);
	}
	else
	{
		SetWraps(wrapOption, wrapOption);
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

void Texture::SetFilters(const unsigned int filterOption) const
{
	SetFilters(filterOption, filterOption);
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