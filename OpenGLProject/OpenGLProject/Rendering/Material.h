#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

#include "ShaderLoader.h"
#include "Texture.h"

class Shader;



struct EmissiveProperties
{
	// No Material Emissive properties for now
};

// Class handling the application of a Shader on a single Mesh (built from scratch, or retrieved from a .mtl Material file for Models)
// Warning: model is supposed to be simple enough, i.e. only contains 1 Mesh for 1 Material definition containing 1 Texture
class Material
{
public:
	// Default constructor (not needed)
	Material() = delete;

	// User-defined constructor (that should be the only one needed at instantiation time)
	Material(const ShaderLookUpID::Enum inShaderLookUpID, const std::vector<Texture>& inTextures, const float inTransparency);

	// Copy constructor (needed as member variable of type Material is present in SceneEntity class)
	Material(const Material& inMaterial) = default;
	Material& operator = (const Material& inMaterial) = delete;

	// Move constructor (needed when moving a Material instance to the SceneEntity member attribute from the Child initialisation-list)
	Material(Material&& inMaterial) = default;
	Material& operator = (Material&& inMaterial) = delete;

	// Virtual destructor (needed to handle any custom polymorphic deletion in child classes)
	virtual ~Material() = default;

	void EnableTextures() const;
	void DisableTextures() const;

	Shader& GetShader() const { return ShaderLoader::GetShader(shaderLookUpID); }

	const std::vector<Texture>& GetTextures() const { return textures; }

protected:
	[[maybe_unused]] EmissiveProperties emissiveProperties;

	ShaderLookUpID::Enum shaderLookUpID;

	// DDS Textures2D to be bound to as many Samplers2D in a Shader where this Material is used (only a single Diffuse Texture2D in most cases)
	std::vector<Texture> textures;

	// Indexes of the Sampler2D slots storing the Texture2Ds of a Shader for look-up operations
	int numOfTextureUnits{ 0 };

	// Coefficient corresponding to the alpha value in a colour vector
	float transparency{ 1.0f };

	void SetFUniforms() const;

	void IncrementTextureUnitCount(int& TextureUnit) const;
};



#endif // MATERIAL_H