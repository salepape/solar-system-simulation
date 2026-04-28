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
	// Shader should already be enabled/disabled in child classes prior to this method call
	Shader& shader = GetShader();

	const std::string modelVU("vu_Model");
	if (shader.IsUniformRequired(modelVU.c_str()))
	{
		shader.SetUniformMat4(modelVU, glm::mat4(0.0f));
	}

	const std::string transparencyFU("material.fu_Transparency");
	if (shader.IsUniformRequired(transparencyFU.c_str()))
	{
		shader.SetUniformFloat(transparencyFU, transparency);
	}
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
