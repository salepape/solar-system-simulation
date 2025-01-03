#include "Texture.h"

#include <glad.h>
#include <iostream>
#include <SOIL2.h>



Texture::Texture(const std::filesystem::path& inPath, const uint32_t inTarget, const WrapOptions& wrapOptions, const FilterOptions& filterOptions, const TextureType& inTextureType) :
	path(inPath), target(inTarget), textureType(inTextureType)
{
	SetWraps(wrapOptions);
	SetFilters(filterOptions);
}

void Texture::LoadFTBitmap(const FT_Bitmap& bitmap, const uint32_t format)
{
	// Generate then bind Texture Object (= TO) to the OpenGL TO type we want 
	glGenTextures(1, &rendererID);

	Bind();

	// @todo - Add 1 texel border to each side to avoid leaking from adjacent glyphs due to linear filtering used?
	// Generate texture image on the currently bound TO at the active texture unit
	glTexImage2D(target, 0, format, bitmap.width, bitmap.rows, 0, format, GL_UNSIGNED_BYTE, bitmap.buffer);

	// Generate all mipmap levels for the previous currently bound TO
	glGenerateMipmap(target);

	Unbind();
}

void Texture::LoadDDS()
{
	// Already contains glGenTextures function call!!!
	rendererID = SOIL_load_OGL_texture(path.string().c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (rendererID == 0)
	{
		std::cout << "ERROR::SOIL - Loading error: " << SOIL_last_result() << std::endl;
	}
}

void Texture::LoadCubemapDDS()
{
	// Already contains glGenTextures function call!!!
	rendererID = SOIL_load_OGL_single_cubemap(path.string().c_str(), "EWUDNS", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (rendererID == 0)
	{
		std::cout << "ERROR::SOIL - Loading error: " << SOIL_last_result() << std::endl;
	}
}

void Texture::SetWraps(const WrapOptions& wrapOptions) const
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapOptions.s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapOptions.t);
	if (target == GL_TEXTURE_CUBE_MAP)
	{
		glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapOptions.r);
	}
}

void Texture::SetFilters(const FilterOptions& filterOptions) const
{
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filterOptions.min);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filterOptions.mag);
}

void Texture::Activate(const uint32_t textureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
}

void Texture::Deactivate() const
{
	// @todo - Find out why this command trigger OpenGL issues when calling glGetError()
	glActiveTexture(0);
}

void Texture::Bind() const
{
	glBindTexture(target, rendererID);
}

void Texture::Unbind() const
{
	glBindTexture(target, 0);
}

void Texture::Enable(const uint32_t textureUnit) const
{
	Activate(textureUnit);
	Bind();
}

void Texture::Disable() const
{
	Deactivate();
	Unbind();
}