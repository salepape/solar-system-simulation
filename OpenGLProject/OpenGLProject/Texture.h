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

	unsigned int rendererID;
	std::string type;
	std::string path;

	// Image loading attributes
	//int width;
	//int height;
	//int nbChannels;

public:
	Texture(std::string path, std::string type, GLenum target, const char * objectType);
	~Texture();

	void LoadTextureImage(GLenum channel);
	void LoadGlyph(FT_Face face, GLenum channel);
	void LoadDDS();
	void LoadCubemapDDS();

	// Set the texture wrapping option (on the currently bound texture object)
	void SetWraps(GLenum wrapType);
	void SetWraps(GLenum s, GLenum t);
	void SetWraps(GLenum s, GLenum t, GLenum r);
	
	// Set the texture filtering option (on the currently bound texture object)
	void SetFilters(GLenum filterType);
	void SetFilters(GLenum min, GLenum mag);

	// Bind texture name to the OpenGL target we want (expl : a 2D texture called GL_TEXTURE_2D)
	void Bind() const;
	void Unbind() const;

	void Enable(unsigned int textUnit);
	void Disable();

	inline unsigned int GetRendererID() const { return rendererID; }
	inline std::string GetPath() const { return path; }
	inline std::string GetType() const { return type; }
};

#endif 