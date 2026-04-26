#include "BlinnPhongMaterial.h"

#include <string>
#include <utility>

#include "Shader.h"



BlinnPhongMaterial::BlinnPhongMaterial(const ShaderLookUpID::Enum inShaderLookUpID, const std::vector<Texture>& inTextures, const DiffuseProperties& inDiffuseProperties, const SpecularProperties& inSpecularProperties, const float inTransparency) :
	Material(inShaderLookUpID, inTextures, inTransparency),
	diffuseProperties(inDiffuseProperties),
	specularProperties(inSpecularProperties)
{
	// Need as many Texture Units as Samplers2D (i.e. Textures2D) for the GLSL Shader used by this Material
	numOfTextureUnits = static_cast<int>(textures.size());

	if (inShaderLookUpID != ShaderLookUpID::UNDEFINED)
	{
		SetFUniforms();
	}
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
		shader.SetUniformInt(diffuseTexFU, diffuseTexFUTextureUnit);
	}
	IncrementTextureUnitCount(diffuseTexFUTextureUnit);

	const std::string diffuseColourFU("material.fu_DiffuseColour");
	if (shader.IsUniformRequired(diffuseColourFU.c_str()))
	{
		shader.SetUniformVec3(diffuseColourFU, diffuseProperties.colour);
	}

	const std::string specularColourFU("material.fu_SpecularColour");
	if (shader.IsUniformRequired(specularColourFU.c_str()))
	{
		shader.SetUniformVec3(specularColourFU, specularProperties.colour);
	}

	const std::string shininessFU("material.fu_Shininess");
	if (shader.IsUniformRequired(shininessFU.c_str()))
	{
		shader.SetUniformFloat(shininessFU, specularProperties.shininess);
	}

	Material::SetFUniforms();

	shader.Disable();
}
