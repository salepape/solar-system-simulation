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
	unsigned int target;

	unsigned int rendererID;
	const char * type;
	const char * path;

	// Image loading attributes
	//int width;
	//int height;
	//int nbChannels;

public:
	Texture(const char * path, const char * type, unsigned int target, const char * objectType);
	~Texture();

	void LoadTextureImage(unsigned int channel);
	void LoadGlyph(FT_Face face, unsigned int channel);
	void LoadDDS();
	void LoadCubemapDDS();

	// Set the texture wrapping option(on the currently bound texture object)
	void SetWraps(unsigned int wrapType);
	void SetWraps(unsigned int s, unsigned int t);
	void SetWraps(unsigned int s, unsigned int t, unsigned int r);
	
	// Set the texture filtering option (on the currently bound texture object)
	void SetFilters(unsigned int filterType);
	void SetFilters(unsigned int min, unsigned int mag);

	void Bind() const;
	void Unbind() const;

	void Enable(unsigned int textUnit);
	void Disable();

	inline unsigned int GetRendererID() const { return rendererID; }
	inline const char * GetPath() const { return path; }
};

#endif 