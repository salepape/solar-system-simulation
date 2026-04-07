#include "PBRMaterial.h"

#include <utility>

#include "Shader.h"



PBRMaterial::PBRMaterial(const ShaderLookUpID::Enum inShaderLookUpID, const std::vector<Texture>& inTextures, const float inTransparency) :
	Material(inShaderLookUpID, inTextures, inTransparency)
{
	// Need as many Texture Units as Samplers2D (i.e. Textures2D) for the GLSL Shader used by this Material
	numOfTextureUnits = static_cast<int>(textures.size());

	SetFUniforms();
}

PBRMaterial::PBRMaterial(PBRMaterial&& inMaterial) :
	Material(inMaterial.shaderLookUpID, std::move(inMaterial.textures), inMaterial.transparency)
{
	// Do not increment Texture Unit count or set Uniforms again!
}

void PBRMaterial::SetFUniforms() const
{
	Shader& shader = GetShader();
	shader.Enable();

	// @todo - PBR uniforms

	Material::SetFUniforms();

	shader.Disable();
}