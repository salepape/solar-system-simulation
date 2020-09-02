#include "Texture.h"





Texture::Texture(const char * pathArg, const char * typeArg, GLenum targetArg, int defaultParams) :
	path(pathArg), type(typeArg), target(targetArg)
{
	// Try catch exception ? if OpenGL context created in main() is still active
	if (glfwGetCurrentContext() == NULL)
	{
		std::cout << "ERROR::GLFW: Failed to get current context : OpenGL functions will not work correctly" << std::endl;
	}

	// Textures by default
	if (defaultParams == 0)
	{
		setWrap(GL_REPEAT);
		setFilters(GL_LINEAR);
	}
	// Textures for characters
	else if (defaultParams == 1)
	{
		generate();

		setWrap(GL_CLAMP_TO_EDGE);
		setFilters(GL_LINEAR);
	}
	// Texture for skybox
	else if (defaultParams == 2)
	{
		setWrap(GL_CLAMP_TO_EDGE);
		setFilters(GL_LINEAR);
	}
}

void Texture::generate()
{
	// Bind TO to the OpenGL texture object (= TO) type we want 
	glGenTextures(1, &textID);

	// Bind texture name to the OpenGL target we want (expl : a 2D texture called GL_TEXTURE_2D)
	glBindTexture(target, textID);
}

void Texture::loadImage(GLenum channel)
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

		//glBindTexture(target, textID);
		bind();

		// Generate texture image on the currently bound TO at the active texture unit
		glTexImage2D(target, 0, format, width, height, nbChannels, format, GL_UNSIGNED_BYTE, data);

		// Generate all mipmap levels for the previous currently bound TO
		glGenerateMipmap(target);
	}
	else
	{
		std::cout << "ERROR::SOIL: Failed to load entity sprite at path " << path << std::endl;
	}

	//stb_image_free(data);
	SOIL_free_image_data(data);
}

void Texture::loadGlyph(FT_Face face, GLenum format)
{
	glTexImage2D(target, 0, format, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, format, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::loadDDS()
{
	// Contains glGenTextures !!!
	textID = SOIL_load_OGL_texture(path, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (0 == textID)
		printf("ERROR::SOIL: Loading error: '%s'\n", SOIL_last_result());

	// Bind texture name to the OpenGL target we want (expl : a 2D texture called GL_TEXTURE_2D)
	//glBindTexture(target, textID);
	bind();
}

void Texture::loadCubemapDDS()
{
	textID = SOIL_load_OGL_single_cubemap(path, "EWUDNS", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_DDS_LOAD_DIRECT);

	if (0 == textID)
		printf("ERROR::SOIL: Loading error: '%s'\n", SOIL_last_result());

	// Bind texture name to the OpenGL target we want (expl : a 2D texture called GL_TEXTURE_2D)
	//glBindTexture(target, textID);
	bind();
}

void Texture::setWrap(GLenum all)
{
	if(target == GL_TEXTURE_CUBE_MAP)
		setWrap(all, all, all);
	else
		setWrap(all, all);
}

void Texture::setWrap(GLenum s, GLenum t)
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
}

void Texture::setWrap(GLenum s, GLenum t, GLenum r)
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, r);
}

void Texture::setFilters(GLenum all)
{
	setFilters(all, all);
}

void Texture::setFilters(GLenum min, GLenum mag)
{
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
}

void Texture::bind() const		// textUnit = samplerID in main() ???
{
	//glActiveTexture(GL_TEXTURE0 + textUnit);
	glBindTexture(target, textID);
}

void Texture::enable(GLint textUnit)		// textUnit = samplerID in main() ???
{
	// Activate the texture unit before binding texture
	glActiveTexture(GL_TEXTURE0 + textUnit);

	bind();
}

void Texture::unbind() const
{
	glBindTexture(target, 0);
}

void Texture::disable()
{
	glActiveTexture(0);
	unbind();
}



