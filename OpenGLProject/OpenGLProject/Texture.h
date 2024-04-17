#ifndef TEXTURE_H
#define TEXTURE_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <string>



// Be careful to keep the same elements order as aiTextureType enum
enum class TextureType
{
	NONE,
	DIFFUSE,
	SPECULAR,
	AMBIANT,
};

struct WrapOptions
{
	const unsigned int s{ 0 };
	const unsigned int t{ 0 };
	const unsigned int r{ 0 };

	WrapOptions(const unsigned int inSame) : s(inSame), t(inSame), r(inSame) {};
	WrapOptions(const unsigned int inS, const unsigned int inT) : s(inS), t(inT) {};
	WrapOptions(const unsigned int inS, const unsigned int inT, const unsigned int inR) : s(inS), t(inT), r(inR) {};
};

struct FilterOptions
{
	const unsigned int min{ 0 };
	const unsigned int mag{ 0 };

	FilterOptions(const unsigned int inSame) : min(inSame), mag(inSame) {};
	FilterOptions(const unsigned int inMin, const unsigned int inMag) : min(inMin), mag(inMag) {};
};

class Texture
{
public:
	Texture();
	Texture(const std::string& inPath, const unsigned int inTarget, const WrapOptions& wrapOptions, const FilterOptions& filterOptions, const TextureType& inTextureType);
	~Texture();

	void LoadFTBitmap(const FT_Bitmap bitmap, const unsigned int channel);
	void LoadDDS();
	void LoadCubemapDDS();

	// Set texture wrapping options on the currently bound texture object (using (s,t,r) texture coordinates)
	void SetWraps(const WrapOptions& wrapOptions) const;

	// Set texture filtering options on the currently bound texture object
	void SetFilters(const FilterOptions& filterOptions) const;

	// Bind texture name to the OpenGL target we want (e.g. a 2D texture called GL_TEXTURE_2D)
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

	// @todo - Use this variable to identify the type of the texture used in a specific model and process samplers accordingly in shaders
	TextureType textureType;
};



#endif // TEXTURE_H