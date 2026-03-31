#include "BodyRings.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "Constants.h"
#include "Renderer.h"
#include "Shader.h"
#include "ShaderLoader.h"



BodyRings::BodyRings(RingsData&& inRingsData) : SceneEntity(inRingsData.bodyName + "Rings", InitialiseParent(inRingsData.opacity)),
model({ inRingsData.modelPath }), ringsData(inRingsData)
{
	material.SetTextures(model.GetTextures());

	bodyName = ringsData.bodyName;
}

Material BodyRings::InitialiseParent(const float inRingsOpacity)
{
	if (inRingsOpacity < 0.5f)
	{
		return Material("InfraredBodyRings", { /* texturesLoadedFromTheModel */ }, { 0, glm::vec3(0.0f) }, { glm::vec3(0.0f), 64.0f }, inRingsOpacity);
	}
	else
	{
		return Material("VisibleBodyRings", { /* texturesLoadedFromTheModel */ }, { 0, glm::vec3(0.0f) }, { glm::vec3(0.0f), 64.0f }, inRingsOpacity);
	}
}

void BodyRings::ComputeModelMatrixVUniform(const glm::mat4& inModelMatrix, const float /*elapsedTime*/)
{
	modelMatrix = inModelMatrix;

	// @todo - Make this model scaling work if possible
	// modelMatrix = glm::scale(modelMatrix, glm::vec3(ringsData.radius));

	// Rotate back (constant over time) around axis normal to orbital plane
	modelMatrix = glm::rotate(modelMatrix, -GLMConstants::halfPi, GLMConstants::rightVector);
}

void BodyRings::Render(const Renderer& renderer, const glm::mat4& inModelMatrix, const float /*elapsedTime*/)
{
	ComputeModelMatrixVUniform(inModelMatrix);

	Shader& shader = material.GetShader();
	shader.Enable();

	SetModelMatrixVUniform();

	material.EnableTextures();
	model.Render(renderer);
	material.DisableTextures();

	shader.Disable();
}