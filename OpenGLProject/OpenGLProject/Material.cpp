#include "Material.h"

#include <iostream>
#include <string>
#include <utility>

#include "Shader.h"



Material::Material(const ShaderLookUpID::Enum inShaderLookUpID, const std::vector<Texture>& inTextures, const DiffuseProperties& inDiffuseProperties, const SpecularProperties& inSpecularProperties, const float inTransparency) :
	shaderLookUpID(inShaderLookUpID), textures(inTextures), diffuseProperties(inDiffuseProperties), specularProperties(inSpecularProperties), transparency(inTransparency)
{
	// Need as many Texture Units as Samplers2D (i.e. Textures2D) for the GLSL Shader used by this Material
	NumOfTextureUnits = static_cast<int>(inTextures.size());

	SetFUniforms();
}

Material::Material(Material&& inMaterial) :
	shaderLookUpID(inMaterial.shaderLookUpID), textures(std::move(inMaterial.textures)), diffuseProperties(std::move(inMaterial.diffuseProperties)), specularProperties(std::move(inMaterial.specularProperties)), transparency(inMaterial.transparency)
{

}

void Material::SetFUniforms() const
{
	Shader& shader = GetShader();
	shader.Enable();

	int diffuseTexFUTextureUnit = 0;
	const std::string& diffuseTexFU = "material.fu_DiffuseTex_" + diffuseTexFUTextureUnit;
	if (shader.IsUniformRequired(diffuseTexFU.c_str()))
	{
		shader.setUniformInt("material.fu_DiffuseTex", diffuseTexFUTextureUnit);
	}
	IncrementTextureUnitCount(diffuseTexFUTextureUnit);

	const std::string& diffuseColourFU = "material.fu_DiffuseColour";
	if (shader.IsUniformRequired(diffuseColourFU.c_str()))
	{
		shader.setUniformVec3(diffuseColourFU, diffuseProperties.emissiveColour);
	}

	const std::string& specularColourFU = "material.fu_SpecularColour";
	if (shader.IsUniformRequired(specularColourFU.c_str()))
	{
		shader.setUniformVec3(specularColourFU, specularProperties.specularColour);
	}

	const std::string& shininessFU = "material.fu_Shininess";
	if (shader.IsUniformRequired(shininessFU.c_str()))
	{
		shader.setUniformFloat(shininessFU, specularProperties.shininess);
	}

	const std::string& transparencyFU = "material.fu_Transparency";
	if (shader.IsUniformRequired(transparencyFU.c_str()))
	{
		shader.setUniformFloat(transparencyFU, transparency);
	}

	shader.Disable();
}

void Material::IncrementTextureUnitCount(int& TextureUnit) const
{
	if (TextureUnit <= NumOfTextureUnits)
	{
		TextureUnit++;
	}
	else
	{
		std::cout << "ERROR::MATERIAL - Max num of Texture Units for this Material (" << NumOfTextureUnits << ") has been exceeded. Please check the size of the 'Textures' vector!" << std::endl;
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