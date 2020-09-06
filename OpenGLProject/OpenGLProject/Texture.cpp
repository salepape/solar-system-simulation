#include "Texture.h"



Texture::Texture(std::string pathArg, std::string typeArg, GLenum targetArg, const char * objectType) :
	path(pathArg), type(typeArg), target(targetArg)
{
	// Texture by default
	if (objectType == "default")
	{
		SetWraps(GL_REPEAT);
		SetFilters(GL_LINEAR);
	}
	// Texture for text characters
	else if (objectType == "text_characters")
	{
		// Generate then bind Texture Object (= TO) to the OpenGL TO type we want 
		glGenTextures(1, &rendererID);
		glBindTexture(target, rendererID);

		SetWraps(GL_CLAMP_TO_EDGE);
		SetFilters(GL_LINEAR);
	}
	// Texture for skybox
	else if (objectType == "skybox")
	{
		SetWraps(GL_CLAMP_TO_EDGE);
		SetFilters(GL_LINEAR);
	}
}

Texture::~Texture()
{
	//glDeleteTextures(1, &rendererID);
}

void Texture::LoadTextureImage(GLenum channel)
{
	int width, height, nbChannels;

	// Load image (without alpha channel)
	unsigned char *data = SOIL_load_image(path.c_str(), &width, &height, &nbChannels, channel);

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
	// Already contains glGenTextures function !!!
	rendererID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (0 == rendererID)
		printf("ERROR::SOIL: Loading error: '%s'\n", SOIL_last_result());

	Bind();
}

void Texture::LoadCubemapDDS()
{
	// Already contains glGenTextures function !!!
	rendererID = SOIL_load_OGL_single_cubemap(path.c_str(), "EWUDNS", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (0 == rendererID)
		printf("ERROR::SOIL: Loading error: '%s'\n", SOIL_last_result());

	Bind();
}

void Texture::SetWraps(GLenum wrapType)
{
	if(target == GL_TEXTURE_CUBE_MAP)
		SetWraps(wrapType, wrapType, wrapType);
	else
		SetWraps(wrapType, wrapType);
}

void Texture::SetWraps(GLenum s, GLenum t)
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
}

void Texture::SetWraps(GLenum s, GLenum t, GLenum r)
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, r);
}

void Texture::SetFilters(GLenum filterType)
{
	SetFilters(filterType, filterType);
}

void Texture::SetFilters(GLenum min, GLenum mag)
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

void Texture::Enable(unsigned int textUnit)		
{
	// Activate the texture unit before binding texture
	glActiveTexture(GL_TEXTURE0 + textUnit);
	Bind();
}

void Texture::Disable()
{
	// Set everything back to default once configured
	glActiveTexture(0);
	Unbind();
}



