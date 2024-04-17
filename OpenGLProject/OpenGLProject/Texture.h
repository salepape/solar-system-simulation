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

class Texture
{
public:
	Texture(const std::string& inPath, const unsigned int inTarget, const unsigned int wrapOption, const unsigned int filterOption, const TextureType inTextureType);
	~Texture();

	void LoadFTBitmap(const FT_Bitmap bitmap, const unsigned int channel);
	void LoadDDS();
	void LoadCubemapDDS();

	// Set the texture wrapping option (on the currently bound texture object) using (s,t,r) texture coordinates
	void SetWraps(const unsigned int wrapOption) const;
	void SetWraps(const unsigned int s, const unsigned int t) const;
	void SetWraps(const unsigned int s, const unsigned int t, const unsigned int r) const;

	// Set the texture filtering option (on the currently bound texture object)
	void SetFilters(const unsigned int filterOption) const;
	void SetFilters(const unsigned int min, const unsigned int mag) const;

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