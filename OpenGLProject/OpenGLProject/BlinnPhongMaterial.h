#ifndef BLINN_PHONG_MATERIAL_H
#define BLINN_PHONG_MATERIAL_H

#include <glm/vec3.hpp>
#include <vector>

#include "Material.h"
#include "ShaderLoader.h"
#include "Texture.h"



struct AmbientProperties
{
	// GLSL Fragment Shaders use Diffuse Texture as Ambient Texture, so no Material Ambient Properties for now
};

struct DiffuseProperties
{
	// Colour of a uniformly-intensed light like the Sun (each coordinate should be comprised in [0.0f, 1.0f])
	glm::vec3 colour{ 0.0f };
};

struct SpecularProperties
{
	// Colour of the specular highlight on the surface (each coordinate should be comprised in [0.0f, 1.0f])
	glm::vec3 colour{ 0.0f };

	// Coefficient altering the radius of the specular highlight
	float shininess{ 64.0f };
};

// Material which lighting model is the Blinn-Phong one
class BlinnPhongMaterial : public Material
{
public:
	// Default constructor (not needed)
	BlinnPhongMaterial() = delete;

	// User-defined constructor (that should be the only one needed at instantiation time)
	BlinnPhongMaterial(const ShaderLookUpID::Enum inShaderLookUpID,
		const std::vector<Texture>& inTextures,
		const DiffuseProperties& inDiffuseProperties = { glm::vec3(0.0f) },
		const SpecularProperties& inSpecularProperties = { glm::vec3(0.0f), 64.0f },
		const float inTransparency = 1.0f);

	// Copy constructor (needed as member variable of type Material is present in SceneEntity class)
	BlinnPhongMaterial(const BlinnPhongMaterial& inMaterial) = default;
	BlinnPhongMaterial& operator = (const BlinnPhongMaterial& inMaterial) = delete;

	// Move constructor (needed when moving a Material instance to the SceneEntity member attribute from the Child initialisation-list)
	BlinnPhongMaterial(BlinnPhongMaterial&& inMaterial);
	BlinnPhongMaterial& operator = (BlinnPhongMaterial&& inMaterial) = delete;

	// Destructor (not virtual needed, until child classes exist)
	~BlinnPhongMaterial() = default;

private:
	[[maybe_unused]] AmbientProperties ambientProperties;
	DiffuseProperties diffuseProperties;
	SpecularProperties specularProperties;

	void SetFUniforms() const;
};



#endif // BLINN_PHONG_MATERIAL_H