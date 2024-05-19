#include "Orbit.h"

#include "Material.h"
#include "ResourceLoader.h"
#include "Shader.h"



// @todo - Build a proper material for the orbits, e.g. a foggy thin tube
Orbit::Orbit(const std::string& texturePath, const float radius) : SceneEntity(Material(ResourceLoader::GetShader("Orbit"))), circle({ texturePath, radius })
{
	name = "Orbit";

	pointLight.Store({ material.GetShader().GetRendererID() });
}

void Orbit::Render(const Renderer& renderer, uint32_t& textureUnit, const glm::mat4& modelMatrix)
{
	Shader& shader = GetMaterial().GetShader();

	shader.Enable();
	GetMaterial().SetDiffuseSamplerVUniform(textureUnit);
	SetModelMatrixUniform(modelMatrix);
	circle.Render(renderer, textureUnit++);
	shader.Disable();
}