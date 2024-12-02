#include "Material.h"

#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "Material.h"
#include "Shader.h"
#include "Utils.h"



Material::Material(Shader& inShader, const std::vector<Texture>& inTextures) :
	shader(inShader), textures(inTextures), diffuseProperties({ 0, glm::vec3(0.0f) }), specularProperties({ glm::vec3(0.0f), 64.0f })
{
	SetFUniforms();
}

Material::Material(Shader& inShader, const std::vector<Texture>& inTextures, const DiffuseProperties& inDiffuseProperties, const float inTransparency) :
	shader(inShader), textures(inTextures), diffuseProperties(inDiffuseProperties), specularProperties({ glm::vec3(0.0f), 64.0f }), transparency(inTransparency)
{
	SetFUniforms();
}

Material::Material(Material&& inMaterial) :
	shader(inMaterial.shader), textures(std::move(inMaterial.textures)), diffuseProperties(std::move(inMaterial.diffuseProperties)), specularProperties(std::move(inMaterial.specularProperties)), transparency(inMaterial.transparency)
{

}

void Material::SetFUniforms()
{
	shader.Enable();

	const std::string& diffuseColourFU = "material.fu_DiffuseColour";
	if (shader.GetUniformLocation(diffuseColourFU.c_str()) != -1)
	{
		shader.setUniformVec3(diffuseColourFU, diffuseProperties.emissiveColour);
	}

	const std::string& specularColourFU = "material.fu_SpecularColour";
	if (shader.GetUniformLocation(specularColourFU.c_str()) != -1)
	{
		shader.setUniformVec3(specularColourFU, specularProperties.emissiveColour);
	}

	const std::string& shininessFU = "material.fu_Shininess";
	if (shader.GetUniformLocation(shininessFU.c_str()) != -1)
	{
		shader.setUniformFloat(shininessFU, specularProperties.shininess);
	}

	const std::string& transparencyFU = "material.fu_Transparency";
	if (shader.GetUniformLocation(transparencyFU.c_str()) != -1)
	{
		shader.setUniformFloat(transparencyFU, transparency);
	}

	shader.Disable();
}

void Material::SetDiffuseSamplerFUniform()
{
	shader.setUniformInt("material.fu_DiffuseTex", diffuseProperties.textureUnit);
}

void Material::SetDiffuseColourFUniform(const glm::vec3& colour)
{
	shader.setUniformVec3("material.fu_DiffuseColour", colour);
}

void Material::EnableTextures()
{
	for (const Texture& texture : textures)
	{
		texture.Enable(diffuseProperties.textureUnit);
	}
}

void Material::DisableTextures()
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