#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "Texture.h"
#include "UniformBuffer.h"

class Shader;



// Material properties of the surface
class Material
{
public:
	Material(Shader& inShader, const std::vector<Texture>& inTextures);
	Material(Shader& inShader, const std::vector<Texture>& inTextures, const glm::vec3& inSpecular, const float inShininess, const float inTransparency = 1.0f);
	Material(Material&& inMaterial);

	void SetFUniforms();
	void SetDiffuseSamplerFUniform();
	void SetDiffuseColourFUniform(const glm::vec3& colour);

	void EnableTextures();
	void DisableTextures();

	Shader& GetShader() { return shader; }
	uint32_t GetDiffuseTextureUnit() const { return diffuseTextureUnit; }

	// Needed for models since we are loading textures using Assimp, done post SceneEntity creation
	void SetTextures(const std::vector<Texture>& inTextures);

private:
	// We need a reference so the shader persists even after Material destruction (Shader exists separately in ResourceLoader)
	Shader& shader;

	// DDS textures that will be used as a 2D samplers (only diffuse textures for now)
	std::vector<Texture> textures;

	// @todo - build a struct to have each texture with its associated unit in the referenced shader
	// Texture unit corresponding to the diffuse texture in the referenced shader
	uint32_t diffuseTextureUnit{ 0 };

	glm::vec3 diffuseColour{ 0.0f, 0.0f, 0.0f };

	// Colour of the specular highlight on the surface (instead of a texture)
	glm::vec3 specularColour{ 0.0f, 0.0f, 0.0f };
	
	// Coefficient altering the radius of the specular highlight
	float shininess{ 64.0f };

	// Coefficient corresponding to the alpha value in a colour vector
	float transparency{ 1.0f };
};



#endif // MATERIAL_H