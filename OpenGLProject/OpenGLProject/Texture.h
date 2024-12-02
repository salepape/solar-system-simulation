#ifndef TEXTURE_H
#define TEXTURE_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <filesystem>
#include <string>



// Be careful to keep the same elements order as aiTextureType enum
enum class TextureType
{
	NONE,
	DIFFUSE,
	SPECULAR,
	AMBIENT,
};

struct WrapOptions
{
	const uint32_t s{ 0 };
	const uint32_t t{ 0 };
	const uint32_t r{ 0 };

	WrapOptions(const uint32_t inSame) : s(inSame), t(inSame), r(inSame) {};
	WrapOptions(const uint32_t inS, const uint32_t inT) : s(inS), t(inT) {};
	WrapOptions(const uint32_t inS, const uint32_t inT, const uint32_t inR) : s(inS), t(inT), r(inR) {};
};

struct FilterOptions
{
	const uint32_t min{ 0 };
	const uint32_t mag{ 0 };

	FilterOptions(const uint32_t inSame) : min(inSame), mag(inSame) {};
	FilterOptions(const uint32_t inMin, const uint32_t inMag) : min(inMin), mag(inMag) {};
};

class Texture
{
public:
	// Default constructor needed for GlyphParams struct declaration
	Texture() = default;
	Texture(const std::filesystem::path& inPath, const uint32_t inTarget, const WrapOptions& wrapOptions, const FilterOptions& filterOptions, const TextureType& inTextureType);

	void LoadFTBitmap(const FT_Bitmap& bitmap, const uint32_t format);
	void LoadDDS();
	void LoadCubemapDDS();

	// Set texture wrapping options on the currently bound texture object (using (s,t,r) texture coordinates)
	void SetWraps(const WrapOptions& wrapOptions) const;

	// Set texture filtering options on the currently bound texture object
	void SetFilters(const FilterOptions& filterOptions) const;

	void Activate(const uint32_t textureUnit) const;
	void Deactivate() const;

	// Bind texture name to the OpenGL target we want (e.g. a 2D texture called GL_TEXTURE_2D)
	void Bind() const;
	void Unbind() const;

	// Activate the texture unit then bind texture
	void Enable(const uint32_t textureUnit) const;
	// Set everything back to default once texture is configured
	void Disable() const;

	uint32_t GetRendererID() const { return rendererID; }
	const std::filesystem::path& GetPath() const { return path; }

private:
	std::filesystem::path path;
	uint32_t target{ 0 };
	uint32_t rendererID{ 0 };

	// @todo - Use this variable to identify the type of the texture used in a specific model and process samplers accordingly in shaders
	TextureType textureType;
};



#endif // TEXTURE_H