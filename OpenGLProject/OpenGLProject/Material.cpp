#include "Material.h"

#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "Shader.h"
#include "Utils.h"



Material::Material(Shader& inShader) : shader(inShader)
{
	SetFUniforms();
}

Material::Material(Shader& inShader, const glm::vec3& inSpecular, const float inShininess, const float inTransparency) :
	shader(inShader), specularColour(inSpecular), shininess(inShininess), transparency(inTransparency)
{
	SetFUniforms();
}

Material::Material(Material&& inMaterial) : shader(inMaterial.shader), specularColour(inMaterial.specularColour), shininess(inMaterial.shininess), transparency(inMaterial.transparency)
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