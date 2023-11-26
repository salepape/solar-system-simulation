#ifndef TEXTURE_H
#define TEXTURE_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <ft2build.h>
#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <SOIL2.h>

#include FT_FREETYPE_H
#define GLFW_INCLUDE_NONE



enum class GeometryType
{
	CIRCLE,

	SPHERE,	
	CUBE,

	CHARACTER,
	MODEL,
};

enum class MapType
{
	NONE,
	DIFFUSE,
	SPECULAR,
	NORMAL,
	HEIGHT,
};

class Texture
{
private:
	unsigned int target{ 0 };
	unsigned int rendererID{ 0 };
	const char* path{ nullptr };
	MapType mapType;

public:
	Texture(const char* path, const unsigned int target, const GeometryType geometryType, const MapType textureType);
	~Texture();

	void LoadTextureImage(const unsigned int channel);
	void LoadGlyph(const FT_Face face, const unsigned int channel);
	void LoadDDS();
	void LoadCubemapDDS();

	// Set the texture wrapping option(on the currently bound texture object)
	void SetWraps(const unsigned int wrapType);
	void SetWraps(const unsigned int s, const unsigned int t);
	void SetWraps(const unsigned int s, const unsigned int t, const unsigned int r);

	// Set the texture filtering option (on the currently bound texture object)
	void SetFilters(const unsigned int filterType);
	void SetFilters(const unsigned int min, const unsigned int mag);

	// Bind texture name to the OpenGL target we want (expl : a 2D texture called GL_TEXTURE_2D)
	void Bind() const;
	void Unbind() const;

	void Enable(unsigned int textUnit);
	void Disable();

	inline unsigned int GetRendererID() const { return rendererID; }
	inline const char* GetPath() const { return path; }
};



#endif // TEXTURE_H