#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "Texture.h"
#include "UniformBuffer.h"

class Shader;



struct DiffuseProperties
{
	// @todo - build a struct to have each texture with its associated unit in the referenced shader, or/and move it to Texture class
	// Texture unit corresponding to the diffuse texture in the referenced shader
	uint32_t textureUnit{ 0 };

	// Used for a uniform emissive intensity like the Sun or glyphs
	glm::vec3 emissiveColour{ 0.0f };
};

struct SpecularProperties
{
	// Colour of the specular highlight on the surface (instead of a texture)
	glm::vec3 emissiveColour{ 0.0f };

	// Coefficient altering the radius of the specular highlight
	float shininess{ 64.0f };
};

// Material properties of the surface
class Material
{
public:
	Material(Shader& inShader, const std::vector<Texture>& inTextures);
	Material(Shader& inShader, const std::vector<Texture>& inTextures, const DiffuseProperties& inDiffuseProperties, const float inTransparency = 1.0f);
	Material(Material&& inMaterial);

	void SetFUniforms();
	void SetDiffuseSamplerFUniform();
	void SetDiffuseColourFUniform(const glm::vec3& colour);

	void EnableTextures();
	void DisableTextures();

	Shader& GetShader() { return shader; }
	uint32_t GetDiffuseTextureUnit() const { return diffuseProperties.textureUnit; }

	// Needed for models since we are loading textures using Assimp, done post SceneEntity creation
	void SetTextures(const std::vector<Texture>& inTextures);

private:
	// We need a reference so the shader persists even after Material destruction (Shader exists separately in ResourceLoader)
	Shader& shader;

	// DDS textures that will be used as a 2D samplers (only diffuse textures for now)
	std::vector<Texture> textures;

	DiffuseProperties diffuseProperties;
	SpecularProperties specularProperties;

	// Coefficient corresponding to the alpha value in a colour vector
	float transparency{ 1.0f };
};



#endif // MATERIAL_H