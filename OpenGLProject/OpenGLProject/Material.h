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

// Class managing the application of a GLSL Shader composed of several Textures2D on a Mesh or a Model 
class Material
{
public:
	Material() = delete;
	Material(const ShaderLookUpID::Enum inShaderLookUpID, const std::vector<Texture>& inTextures, const DiffuseProperties& inDiffuseProperties = { glm::vec3(0.0f) }, const SpecularProperties& inSpecularProperties = { glm::vec3(0.0f), 64.0f }, const float inTransparency = 1.0f);

	Material(const Material& inMaterial) = delete;
	Material& operator = (const Material& inMaterial) = delete;

	// Needed when moving the Material instance to the SceneEntity member attribute from the Child initialisation-list
	Material(Material&& inMaterial);
	Material& operator = (Material&& inMaterial) = delete;

	virtual ~Material() = default;

	void EnableTextures() const;
	void DisableTextures() const;

	Shader& GetShader() const { return ShaderLoader::GetShader(shaderLookUpID); }

	// Needed for models since we are loading textures using Assimp, done post SceneEntity creation
	void SetTextures(const std::vector<Texture>& inTextures);

private:
	ShaderLookUpID::Enum shaderLookUpID;

	// DDS Ttextures2D to be bound to as many Samplers2D in a Shader where this Material is used
	std::vector<Texture> textures;

	// Indexes of the Sampler2D slots storing the Texture2Ds of a Shader for look-up operations
	int NumOfTextureUnits{ 0 };

	DiffuseProperties diffuseProperties;
	SpecularProperties specularProperties;

	// Coefficient corresponding to the alpha value in a colour vector
	float transparency{ 1.0f };

	void SetFUniforms() const;

	void IncrementTextureUnitCount(int& TextureUnit) const;
};



#endif // MATERIAL_H