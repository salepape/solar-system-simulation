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
private:
	GLenum target;

	// Image loading attributes
	//int width;
	//int height;
	//int nbChannels;

public:
	GLuint textID;
	const char * type;
	const char * path;

	Texture(const char * path, const char * type, GLenum target, int defaultParams);

	void Generate();

	void LoadImage(GLenum channel);
	void LoadGlyph(FT_Face face, GLenum channel);
	void LoadDDS();
	void LoadCubemapDDS();

	// Set the texture wrapping option(on the currently bound texture object)
	void SetWrap(GLenum all);
	void SetWrap(GLenum s, GLenum t);
	void SetWrap(GLenum s, GLenum t, GLenum r);
	
	// Set the texture filtering option (on the currently bound texture object)
	void SetFilters(GLenum all);
	void SetFilters(GLenum min, GLenum mag);

	void Bind() const;
	void Enable(GLint textUnit);
	void Unbind() const;
	void Disable();
};

#endif 