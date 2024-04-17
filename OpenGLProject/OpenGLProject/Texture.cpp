#include "Texture.h"

#include <glad.h>
#include <SOIL2.h>



Texture::Texture()
{

}

Texture::Texture(const std::string& inPath, const unsigned int inTarget, const WrapOptions& wrapOptions, const FilterOptions& filterOptions, const TextureType& inTextureType) :
	path(inPath), target(inTarget), textureType(inTextureType)
{
	SetWraps(wrapOptions);
	SetFilters(filterOptions);
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

void Texture::SetWraps(const WrapOptions& wrapOptions) const
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapOptions.s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapOptions.t);

	if (target == GL_TEXTURE_CUBE_MAP)
	{
		glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapOptions.r);
	}
}

void Texture::SetFilters(const FilterOptions& filterOptions) const
{
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filterOptions.min);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filterOptions.mag);
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