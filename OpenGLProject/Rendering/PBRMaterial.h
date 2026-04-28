#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include <vector>

#include "Material.h"
#include "ShaderLoader.h"
#include "Texture.h"



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



#endif // PBR_MATERIAL_H
