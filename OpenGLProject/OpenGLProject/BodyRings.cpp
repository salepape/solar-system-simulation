#include "BodyRings.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Utils.h"



BodyRings::BodyRings(const std::string& modelPath, const std::string& inBodyName) : SceneEntity(InitialiseParent("")),
model({ modelPath })
{
	material.SetTextures(model.GetTextures());

	name = "BodyRings";
	bodyName = inBodyName;
}

Material BodyRings::InitialiseParent(const std::string& inTexturePath)
{
	return Material(ResourceLoader::GetShader("BodyRings"), { /* texturesLoadedFromTheModel */ }, { 0, glm::vec3(0.0f) }, 0.5f);
}

void BodyRings::ComputeModelMatrixVUniform(const float /*elapsedTime*/)
{
	modelMatrix = ResourceLoader::GetBody(bodyName).GetModelMatrix();

	// Rotate back (constant over time) around axis normal to orbital plane
	modelMatrix = glm::rotate(modelMatrix, -Utils::halfPi, Utils::rightVector);
}

void BodyRings::Render(const Renderer& renderer, const float /*elapsedTime*/)
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