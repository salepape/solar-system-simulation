#include "Texture.h"





Texture::Texture(const char * pathArg, const char * typeArg, GLenum targetArg, std::string objectType) :
	path(pathArg), type(typeArg), target(targetArg)
{
	// Textures by default
	if (objectType == "default")
	{
		SetWrap(GL_REPEAT);
		SetFilters(GL_LINEAR);
	}
	// Textures for characters
	else if (objectType == "text_characters")
	{
		Generate();

		SetWrap(GL_CLAMP_TO_EDGE);
		SetFilters(GL_LINEAR);
	}
	// Texture for skybox
	else if (objectType == "skybox")
	{
		SetWrap(GL_CLAMP_TO_EDGE);
		SetFilters(GL_LINEAR);
	}
}

void Texture::Generate()
{
	// Bind TO to the OpenGL texture object (= TO) type we want 
	glGenTextures(1, &rendererID);

	Bind();
}

void Texture::LoadImage(GLenum channel)
{
	int width, height, nbChannels;

	// Load image (without alpha channel)
	unsigned char *data = SOIL_load_image(path, &width, &height, &nbChannels, channel);

	if (data)
	{
		GLenum format;
		switch (nbChannels)
		{
		case 1 : 
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}

		Bind();

		// Generate texture image on the currently bound TO at the active texture unit
		glTexImage2D(target, 0, format, width, height, nbChannels, format, GL_UNSIGNED_BYTE, data);

		// Generate all mipmap levels for the previous currently bound TO
		glGenerateMipmap(target);
	}
	else
	{
		std::cout << "ERROR::SOIL: Failed to load entity sprite at path " << path << std::endl;
	}

	SOIL_free_image_data(data);
}

void Texture::LoadGlyph(FT_Face face, GLenum format)
{
	glTexImage2D(target, 0, format, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, format, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::LoadDDS()
{
	// Contains already glGenTextures !!!
	rendererID = SOIL_load_OGL_texture(path, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (0 == rendererID)
		printf("ERROR::SOIL: Loading error: '%s'\n", SOIL_last_result());

	Bind();
}

void Texture::LoadCubemapDDS()
{
	// Contains already glGenTextures !!!
	rendererID = SOIL_load_OGL_single_cubemap(path, "EWUDNS", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (0 == rendererID)
		printf("ERROR::SOIL: Loading error: '%s'\n", SOIL_last_result());

	Bind();
}

void Texture::SetWrap(GLenum all)
{
	if(target == GL_TEXTURE_CUBE_MAP)
		SetWrap(all, all, all);
	else
		SetWrap(all, all);
}

void Texture::SetWrap(GLenum s, GLenum t)
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
}

void Texture::SetWrap(GLenum s, GLenum t, GLenum r)
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, r);
}

void Texture::SetFilters(GLenum all)
{
	SetFilters(all, all);
}

void Texture::SetFilters(GLenum min, GLenum mag)
{
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
}

Texture::~Texture()
{

}

void Texture::Bind() const		
{
	// Bind texture name to the OpenGL target we want (expl : a 2D texture called GL_TEXTURE_2D)
	glBindTexture(target, rendererID);
}

void Texture::Enable(GLint textUnit)		
{
	// Activate the texture unit before binding texture
	glActiveTexture(GL_TEXTURE0 + textUnit);
	Bind();
}

void Texture::Unbind() const
{
	glBindTexture(target, 0);
}

void Texture::Disable()
{
	glActiveTexture(0);
	Unbind();
}



