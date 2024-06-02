#include "SaturnRings.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Utils.h"



SaturnRings::SaturnRings(const std::string& modelPath) : SceneEntity(InitialiseParent("")),
model({ modelPath })
{
	material.SetTextures(model.GetTextures());

	name = "SaturnRings";
}

Material SaturnRings::InitialiseParent(const std::string& inTexturePath)
{
	return Material(ResourceLoader::GetShader("SaturnRings"), { /* texturesLoadedFromTheModel */ }, glm::vec3(0.0f, 0.0f, 0.0f), 64.0f, 0.5f);
}

void SaturnRings::ComputeModelMatrixVUniform(const float /*elapsedTime*/)
{
	modelMatrix = ResourceLoader::GetBody("Saturn").GetModelMatrix();

	// Rotate back (constant over time) around axis normal to orbital plane
	modelMatrix = glm::rotate(modelMatrix, -Utils::halfPi, Utils::rightVector);
}

void SaturnRings::Render(const Renderer& renderer, const float /*elapsedTime*/)
{
	ComputeModelMatrixVUniform();

	Shader& shader = material.GetShader();
	shader.Enable();

	SetModelMatrixVUniform(modelMatrix);

	material.SetDiffuseSamplerFUniform();

	material.EnableTextures();
	model.Render(renderer);
	material.DisableTextures();

	shader.Disable();
}