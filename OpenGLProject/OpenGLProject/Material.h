#ifndef MATERIAL_H
#define MATERIAL_H

#include <cstdint>
#include <glm/vec3.hpp>
#include <vector>

#include "Shader.h"
#include "ShaderLoader.h"
#include "Texture.h"



struct DiffuseProperties
{
	// @todo - Build a struct to have each texture with its associated unit in the referenced shader, or/and move it to Texture class
	// Texture unit corresponding to the diffuse texture in the referenced shader
	uint32_t textureUnit{ 0 };

	// Used for a uniform emissive intensity like the Sun or glyphs
	glm::vec3 emissiveColour{ 0.0f };
};

struct SpecularProperties
{
	// Colour of the specular highlight on the surface (instead of a texture)
	glm::vec3 specularColour{ 0.0f };

	// Coefficient altering the radius of the specular highlight
	float shininess{ 64.0f };
};

// Material properties of the surface
class Material
{
public:
	Material() = delete;
	Material(const ShaderLookUpID::Enum inShaderLookUpID, const std::vector<Texture>& inTextures, const DiffuseProperties& inDiffuseProperties = { 0, glm::vec3(0.0f) }, const SpecularProperties& inSpecularProperties = { glm::vec3(0.0f), 64.0f }, const float inTransparency = 1.0f);

	Material(const Material& inMaterial) = delete;
	Material& operator = (const Material& inMaterial) = delete;

	Material(Material&& inMaterial) = delete;
	Material& operator = (Material&& inMaterial) = delete;

	virtual ~Material() = default;

	void EnableTextures() const;
	void DisableTextures() const;

	Shader& GetShader() const { return ShaderLoader::GetShader(shaderLookUpID); }
	uint32_t GetDiffuseTextureUnit() const { return diffuseProperties.textureUnit; }

	// Needed for models since we are loading textures using Assimp, done post SceneEntity creation
	void SetTextures(const std::vector<Texture>& inTextures);

private:
	ShaderLookUpID::Enum shaderLookUpID;

	// DDS textures that will be used as a 2D samplers (only diffuse textures for now)
	std::vector<Texture> textures;

	DiffuseProperties diffuseProperties;
	SpecularProperties specularProperties;

	// Coefficient corresponding to the alpha value in a colour vector
	float transparency{ 1.0f };

	void SetFUniforms() const;
};



#endif // MATERIAL_H