#ifndef TEXTURE_H
#define TEXTURE_H

#define GLFW_INCLUDE_NONE
#include <glfw3.h>

#include <glad.h>
#include <SOIL2.h>

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>	

#include <ft2build.h>
#include FT_FREETYPE_H

class Texture
{
public:
	GLuint textID;
	const char * type;
	const char * path;

	Texture(const char * path, const char * type, GLenum target, int defaultParams);

	void generate();

	void loadImage(GLenum channel);
	void loadGlyph(FT_Face face, GLenum channel);
	void loadDDS();
	void loadCubemapDDS();

	// Set the texture wrapping option(on the currently bound texture object)
	void setWrap(GLenum all);
	void setWrap(GLenum s, GLenum t);
	void setWrap(GLenum s, GLenum t, GLenum r);
	
	// Set the texture filtering option (on the currently bound texture object)
	void setFilters(GLenum all);
	void setFilters(GLenum min, GLenum mag);

	void bind();
	void enable(GLint textUnit);
	void unbind();
	void disable();



private:
	GLenum target;

	// Image loading attributes
	//int width;
	//int height;
	//int nbChannels;
};

#endif 