#ifndef MATERIAL_H
#define MATERIAL_H

#include <cstdint>
#include <glm/vec3.hpp>
#include <vector>

#include "ShaderLoader.h"
#include "Texture.h"

class Shader;



struct EmissiveProperties
{
	// No Material Emissive properties for now
};

// Class handling the application of a Shader on a single Mesh (built from scratch, or retrieved from .mtl Model file)
// Warning: Model is supposed to be simple enough, i.e. only contains 1 Mesh for 1 Material definition containing 1 Texture
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

	// Needed for Models since we are loading textures from .mtl files using ASSIMP, which is performed post SceneEntity creation in the initialisation-list
	void SetTextures(const std::vector<Texture>& inTextures);
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



struct AmbientProperties
{
	// GLSL Fragment Shaders use Diffuse Texture as Ambient Texture, so no Material Ambient Properties for now
};

struct DiffuseProperties
{
	// Used for a uniform light intensity like the Sun
	glm::vec3 colour{ 0.0f };
};

struct SpecularProperties
{
	// Colour of the specular highlight on the surface (instead of using a Texture2D)
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



struct AlbedoProperties
{

};

struct MetalnessProperties
{

};

struct RoughnessProperties
{

};

struct AmbientOcclusionProperties
{

};

// Material which lighting model is based on the Physically-Based Rendering (PBR) theory
class PBRMaterial : public Material
{
public:
	// Default constructor (not needed)
	PBRMaterial() = delete;

	// User-defined constructor (that should be the only one needed at instantiation time)
	PBRMaterial(const ShaderLookUpID::Enum inShaderLookUpID,
		const std::vector<Texture>& inTextures,
		const float inTransparency = 1.0f);

	// Copy constructor (needed as member variable of type Material is present in SceneEntity class)
	PBRMaterial(const PBRMaterial& inMaterial) = default;
	PBRMaterial& operator = (const PBRMaterial& inMaterial) = delete;

	// Move constructor (needed when moving a Material instance to the SceneEntity member attribute from the Child initialisation-list)
	PBRMaterial(PBRMaterial&& inMaterial);
	PBRMaterial& operator = (PBRMaterial&& inMaterial) = delete;

	// Destructor (not virtual needed, until child classes exist)
	~PBRMaterial() = default;

private:
	[[maybe_unused]] AlbedoProperties albedoProperties;
	[[maybe_unused]] MetalnessProperties metalnessProperties;
	[[maybe_unused]] RoughnessProperties roughnessProperties;
	[[maybe_unused]] AmbientOcclusionProperties aoProperties;

	void SetFUniforms() const;
};



#endif // MATERIAL_H