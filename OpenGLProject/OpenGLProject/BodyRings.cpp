#include "BodyRings.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Utils.h"



BodyRings::BodyRings(RingsData&& inRingsData) : SceneEntity(InitialiseParent(inRingsData.opacity)),
model({ inRingsData.modelPath }), ringsData(inRingsData)
{
	material.SetTextures(model.GetTextures());

	name = "BodyRings";
	bodyName = ringsData.bodyName;
}

Material BodyRings::InitialiseParent(const float inRingsOpacity)
{
	if (inRingsOpacity < 0.5f)
	{
		return Material(ResourceLoader::GetShader("InfraredBodyRings"), { /* texturesLoadedFromTheModel */ }, { 0, glm::vec3(0.0f) }, inRingsOpacity);
	}
	else
	{
		return Material(ResourceLoader::GetShader("VisibleBodyRings"), { /* texturesLoadedFromTheModel */ }, { 0, glm::vec3(0.0f) }, inRingsOpacity);
	}
}

void BodyRings::ComputeModelMatrixVUniform(const float /*elapsedTime*/)
{
	modelMatrix = ResourceLoader::GetBodySystem(bodyName).celestialBody.GetModelMatrix();

	// @todo - Make this model scaling work if possible
	// modelMatrix = glm::scale(modelMatrix, glm::vec3(ringsData.radius));

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