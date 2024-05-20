#include "Material.h"

#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "Shader.h"
#include "Utils.h"

uint32_t Material::diffuseSamplerIDCounter = 0;



Material::Material(Shader& inShader) : shader(inShader)
{
	diffuseSamplerID = Material::diffuseSamplerIDCounter++;

	Store({ shader.GetRendererID() });
}

Material::Material(Shader& inShader, const glm::vec3& inSpecular, const float inShininess, const float inTransparency) :
	shader(inShader), specularColour(inSpecular), shininess(inShininess), transparency(inTransparency)
{
	Store({ shader.GetRendererID() });
}

Material::Material(Material&& inMaterial) : shader(inMaterial.shader), specularColour(inMaterial.specularColour), shininess(inMaterial.shininess), transparency(inMaterial.transparency), ubo(std::move(inMaterial.ubo))
{

}

void Material::Store(const std::vector<uint32_t>& entitiesShadersIDs)
{
	ubo = std::make_unique<UniformBuffer>(entitiesShadersIDs, "specularMatParams", Utils::vec4Size + 2 * Utils::scalarSize);
	ubo->InitSubData({
		{ static_cast<const void*>(glm::value_ptr(specularColour)), Utils::vec4Size },
		{ static_cast<const void*>(&shininess), Utils::scalarSize },
		{ static_cast<const void*>(&transparency), Utils::scalarSize }
		});
}

void Material::SetDiffuseSamplerVUniform()
{
	shader.setUniformInt("fu_DiffuseMat", diffuseSamplerID);
}

void Material::SetDiffuseColourVUniform(const glm::vec3& colour)
{
	shader.setUniformVec3("fu_DiffuseColour", colour);
}