#ifndef TEXTURE_H
#define TEXTURE_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <string>

enum class GeometryType;
enum class MapType;



enum class GeometryType
{
	CIRCLE,

	SPHERE,	
	CUBE,

	GLYPH,
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
public:
	Texture(const std::string& path, const unsigned int target, const GeometryType geometryType, const MapType textureType);
	~Texture();

	void LoadSprite(const unsigned int channel) const;
	void LoadFTBitmap(const FT_Bitmap bitmap, const unsigned int channel) const;
	void LoadDDS();
	void LoadCubemapDDS();

	// Set the texture wrapping option (on the currently bound texture object) using (s,t,r) texture coordinates
	void SetWraps(const unsigned int wrapType) const;
	void SetWraps(const unsigned int s, const unsigned int t) const;
	void SetWraps(const unsigned int s, const unsigned int t, const unsigned int r) const;

	// Set the texture filtering option (on the currently bound texture object)
	void SetFilters(const unsigned int filterType) const;
	void SetFilters(const unsigned int min, const unsigned int mag) const;

	// Bind texture name to the OpenGL target we want (expl : a 2D texture called GL_TEXTURE_2D)
	void Bind() const;
	void Unbind() const;

	// Activate the texture unit then bind texture
	void Enable(unsigned int textUnit) const;

	// Set everything back to default once texture is configured
	void Disable() const;

	unsigned int GetRendererID() const { return rendererID; }
	const std::string& GetPath() const { return path; }

private:
	unsigned int target{ 0 };
	unsigned int rendererID{ 0 };
	std::string path;
	MapType mapType;
};



#endif // TEXTURE_H