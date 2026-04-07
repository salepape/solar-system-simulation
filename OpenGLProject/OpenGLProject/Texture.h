#ifndef TEXTURE_H
#define TEXTURE_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <array>
#include <assimp/material.h>
#include <cstdint>
#include <filesystem>



// Warning: keep the same order of elements as aiTextureType enum, to ensure of a correct mapping between the 2 enums when casting
namespace TextureType
{
	enum Enum
	{
		NONE = aiTextureType_NONE,

		// Texture types used to define Materials using the Blinn-Phong lighting model
		DIFFUSE = aiTextureType_DIFFUSE,
		SPECULAR = aiTextureType_SPECULAR,
		AMBIENT = aiTextureType_AMBIENT,
		EMISSIVE = aiTextureType_EMISSIVE,

		//// Texture types used to define Materials using the PBR lighting model
		//ALBEDO = aiTextureType_BASE_COLOR,
		//EMISSIVE = aiTextureType_EMISSION_COLOR,
		//METALNESS = aiTextureType_METALNESS,
		//ROUGHNESS = aiTextureType_DIFFUSE_ROUGHNESS,
		//AO = aiTextureType_AMBIENT_OCCLUSION,
	};

	static const std::array<Enum, 5> All = { NONE, DIFFUSE, SPECULAR, AMBIENT, EMISSIVE, };
};

struct WrapOptions
{
	uint32_t s{ 0 };
	uint32_t t{ 0 };
	uint32_t r{ 0 };

	WrapOptions(const uint32_t inSame) : s(inSame), t(inSame), r(inSame) {};
	WrapOptions(const uint32_t inS, const uint32_t inT) : s(inS), t(inT) {};
	WrapOptions(const uint32_t inS, const uint32_t inT, const uint32_t inR) : s(inS), t(inT), r(inR) {};
};

struct FilterOptions
{
	uint32_t min{ 0 };
	uint32_t mag{ 0 };

	FilterOptions(const uint32_t inSame) : min(inSame), mag(inSame) {};
	FilterOptions(const uint32_t inMin, const uint32_t inMag) : min(inMin), mag(inMag) {};
};

class Texture
{
public:
	// Default constructor (not needed)
	Texture() = delete;

	// User-defined constructor (to be used when instantiating a Texture instance in usual circumstances)
	Texture(const std::filesystem::path& inImagePath, const uint32_t inTarget, WrapOptions&& wrapOptions, FilterOptions&& filterOptions, const TextureType::Enum inTextureType);

	// Copy constructor (needed when copying the whole Texture vector)
	Texture(const Texture& inTexture) = default;
	Texture& operator = (const Texture& inTexture) = delete;

	// Move constructor (needed when moving Texture in Glyph struct after being defined)
	Texture(Texture&& inTexture) = default;
	Texture&& operator = (Texture&& inTexture) = delete;

	// Destructor (not virtual needed, until child classes exist)
	~Texture() = default;

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
	const std::filesystem::path& GetImagePath() const { return imagePath; }

private:
	std::filesystem::path imagePath;
	uint32_t target{ 0 };
	uint32_t rendererID{ 0 };

	[[maybe_unused]] TextureType::Enum textureType{ TextureType::Enum::NONE };
};



#endif // TEXTURE_H