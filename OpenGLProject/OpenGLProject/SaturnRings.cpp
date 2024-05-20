#include "SaturnRings.h"

#include "Material.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"



SaturnRings::SaturnRings(const std::string& texturePath) : SceneEntity(Material(ResourceLoader::GetShader("SaturnRings"), { 0.0f, 0.0f, 0.0f }, 64.0f, 0.5f)), model({ texturePath })
{
	name = "SaturnRings";

	pointLight.Store({ material.GetShader().GetRendererID() });
}

void SaturnRings::Render(const Renderer& renderer, uint32_t& textureUnit, const glm::mat4& modelMatrix)
{
	Shader& shader = material.GetShader();

	shader.Enable();
	material.SetDiffuseSamplerVUniform(textureUnit);
	SetModelMatrixUniform(modelMatrix);
	model.Render(renderer, textureUnit++);
	shader.Disable();
}