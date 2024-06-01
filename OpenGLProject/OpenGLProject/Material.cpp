#include "Material.h"

#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "Material.h"
#include "Shader.h"
#include "Utils.h"



Material::Material(Shader& inShader, const std::vector<Texture>& inTextures) :
	shader(inShader), textures(inTextures), diffuseTextureUnit(0)
{
	SetFUniforms();
}

Material::Material(Shader& inShader, const std::vector<Texture>& inTextures, const glm::vec3& inSpecular, const float inShininess, const float inTransparency) :
	shader(inShader), textures(inTextures), diffuseTextureUnit(0), specularColour(inSpecular), shininess(inShininess), transparency(inTransparency)
{
	SetFUniforms();
}

Material::Material(Material&& inMaterial) :
	shader(inMaterial.shader), textures(std::move(inMaterial.textures)), diffuseTextureUnit(0), specularColour(inMaterial.specularColour), shininess(inMaterial.shininess), transparency(inMaterial.transparency)
{

}

void Material::SetFUniforms()
{
	shader.Enable();

	const std::string& unif1 = "material.fu_SpecularColour";
	if (shader.GetUniformLocation(unif1.c_str()) != -1)
	{
		shader.setUniformVec3(unif1, specularColour);
	}

	const std::string& unif2 = "material.fu_Shininess";
	if (shader.GetUniformLocation(unif2.c_str()) != -1)
	{
		shader.setUniformFloat(unif2, shininess);
	}

	const std::string& unif3 = "material.fu_Transparency";
	if (shader.GetUniformLocation(unif3.c_str()) != -1)
	{
		shader.setUniformFloat(unif3, transparency);
	}

	shader.Disable();
}

void Material::SetDiffuseSamplerFUniform()
{
	shader.setUniformInt("material.fu_DiffuseMat", diffuseTextureUnit);
}

void Material::SetDiffuseColourFUniform(const glm::vec3& colour)
{
	shader.setUniformVec3("material.fu_DiffuseColour", colour);
}

void Material::EnableTextures()
{
	for (const auto& texture : textures)
	{
		texture.Enable(diffuseTextureUnit);
	}
}

void Material::DisableTextures()
{
	for (const auto& texture : textures)
	{
		texture.Disable();
	}
}

void Material::SetTextures(const std::vector<Texture>& inTextures)
{
	textures = inTextures;
}