#include "SaturnRings.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include "Material.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Utils.h"



SaturnRings::SaturnRings(const std::string& texturePath) : SceneEntity(Material(ResourceLoader::GetShader("SaturnRings"), { 0.0f, 0.0f, 0.0f }, 64.0f, 0.5f)), model({ texturePath })
{
	name = "SaturnRings";

	pointLight.Store({ material.GetShader().GetRendererID() });
}

void SaturnRings::ComputeModelMatrixUniform(const float /*elapsedTime*/)
{
	modelMatrix = ResourceLoader::GetBody("Saturn").GetModelMatrix();

	// Rotate back (constant over time) around axis normal to orbital plane
	modelMatrix = glm::rotate(modelMatrix, -Utils::halfPi, Utils::rightVector);
}

void SaturnRings::Render(const Renderer& renderer, const float /*elapsedTime*/)
{
	ComputeModelMatrixUniform();

	Shader& shader = material.GetShader();

	shader.Enable();
	material.SetDiffuseSamplerVUniform();
	SetModelMatrixUniform(modelMatrix);
	model.Render(renderer, material.GetDiffuseSamplerID());
	shader.Disable();
}