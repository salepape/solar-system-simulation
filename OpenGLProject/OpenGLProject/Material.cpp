#include "Material.h"

#include <iostream>
#include <string>
#include <utility>

#include "Shader.h"



Material::Material(const ShaderLookUpID::Enum inShaderLookUpID, const std::vector<Texture>& inTextures, const float inTransparency) :
	shaderLookUpID(inShaderLookUpID),
	textures(inTextures),
	transparency(inTransparency)
{

}

void Material::SetFUniforms() const
{
	Shader& shader = GetShader();

	const std::string transparencyFU("material.fu_Transparency");
	if (shader.IsUniformRequired(transparencyFU.c_str()))
	{
		shader.setUniformFloat(transparencyFU, transparency);
	}

	shader.Disable();
}

void Material::IncrementTextureUnitCount(int& textureUnit) const
{
	if (textureUnit <= numOfTextureUnits)
	{
		textureUnit++;
	}
	else
	{
		std::cout << "ERROR::MATERIAL - Max num of Texture Units for this Material (" << numOfTextureUnits << ") has been exceeded. Please check the size of the 'Textures' vector!" << std::endl;
	}
}

void Material::EnableTextures() const
{
	int textureUnit = 0;
	for (const Texture& texture : textures)
	{
		texture.Enable(textureUnit);
		IncrementTextureUnitCount(textureUnit);
	}
}

void Material::DisableTextures() const
{
	for (const Texture& texture : textures)
	{
		texture.Disable();
	}
}

void Material::SetTextures(const std::vector<Texture>& inTextures)
{
	textures = inTextures;
}



BlinnPhongMaterial::BlinnPhongMaterial(const ShaderLookUpID::Enum inShaderLookUpID, const std::vector<Texture>& inTextures, const DiffuseProperties& inDiffuseProperties, const SpecularProperties& inSpecularProperties, const float inTransparency) :
	Material(inShaderLookUpID, inTextures, inTransparency),
	diffuseProperties(inDiffuseProperties),
	specularProperties(inSpecularProperties)
{
	// Need as many Texture Units as Samplers2D (i.e. Textures2D) for the GLSL Shader used by this Material
	numOfTextureUnits = static_cast<int>(textures.size());

	SetFUniforms();
}

BlinnPhongMaterial::BlinnPhongMaterial(BlinnPhongMaterial&& inMaterial) :
	Material(inMaterial.shaderLookUpID, std::move(inMaterial.textures), inMaterial.transparency),
	diffuseProperties(std::move(inMaterial.diffuseProperties)),
	specularProperties(std::move(inMaterial.specularProperties))
{
	// Do not increment Texture Unit count or set Uniforms again!
}

void BlinnPhongMaterial::SetFUniforms() const
{
	Shader& shader = GetShader();
	shader.Enable();

	int diffuseTexFUTextureUnit = 0;
	const std::string diffuseTexFU("material.fu_DiffuseTex_" + std::to_string(diffuseTexFUTextureUnit));
	if (shader.IsUniformRequired(diffuseTexFU.c_str()))
	{
		shader.setUniformInt(diffuseTexFU, diffuseTexFUTextureUnit);
	}
	IncrementTextureUnitCount(diffuseTexFUTextureUnit);

	const std::string diffuseColourFU("material.fu_DiffuseColour");
	if (shader.IsUniformRequired(diffuseColourFU.c_str()))
	{
		shader.setUniformVec3(diffuseColourFU, diffuseProperties.colour);
	}

	const std::string specularColourFU("material.fu_SpecularColour");
	if (shader.IsUniformRequired(specularColourFU.c_str()))
	{
		shader.setUniformVec3(specularColourFU, specularProperties.colour);
	}

	const std::string shininessFU("material.fu_Shininess");
	if (shader.IsUniformRequired(shininessFU.c_str()))
	{
		shader.setUniformFloat(shininessFU, specularProperties.shininess);
	}

	Material::SetFUniforms();

	shader.Disable();
}



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
	// @todo - PBR uniforms

	Material::SetFUniforms();
}