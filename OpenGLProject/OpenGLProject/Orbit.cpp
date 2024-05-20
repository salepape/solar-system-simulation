#include "Orbit.h"

#include "Material.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"



// @todo - Build a proper material for the orbits, e.g. a foggy thin tube
Orbit::Orbit(const std::string& texturePath, const float radius) : SceneEntity(Material(ResourceLoader::GetShader("Orbit"))), circle({ texturePath, radius })
{
	name = "Orbit";

	pointLight.Store({ material.GetShader().GetRendererID() });
}

void Orbit::Render(const Renderer& renderer, const glm::mat4& modelMatrix)
{
	Shader& shader = material.GetShader();

	shader.Enable();
	material.SetDiffuseSamplerVUniform();
	SetModelMatrixUniform(modelMatrix);
	circle.Render(renderer, material.GetDiffuseSamplerID());
	shader.Disable();
}