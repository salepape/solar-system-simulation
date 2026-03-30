#include "Material.h"

#include <string>
#include <utility>

#include "Shader.h"



Material::Material(Shader& inShader, const std::vector<Texture>& inTextures, const DiffuseProperties& inDiffuseProperties, const SpecularProperties& inSpecularProperties, const float inTransparency) :
	shader(inShader), textures(inTextures), diffuseProperties(inDiffuseProperties), specularProperties(inSpecularProperties), transparency(inTransparency)
{
	SetFUniforms();
}

Material::Material(Material&& inMaterial) :
	shader(inMaterial.shader), textures(std::move(inMaterial.textures)), diffuseProperties(std::move(inMaterial.diffuseProperties)), specularProperties(std::move(inMaterial.specularProperties)), transparency(inMaterial.transparency)
{

}

void Material::SetFUniforms() const
{
	shader.Enable();

	const std::string& diffuseTexFU = "material.fu_DiffuseTex";
	if (shader.IsUniformRequired(diffuseTexFU.c_str()))
	{
		shader.setUniformInt("material.fu_DiffuseTex", diffuseProperties.textureUnit);
	}

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

void Material::EnableTextures() const
{
	for (const Texture& texture : textures)
	{
		texture.Enable(diffuseProperties.textureUnit);
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