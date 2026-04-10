#include "BodyRings.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "BlinnPhongMaterial.h"
#include "Constants.h"
#include "Renderer.h"
#include "Shader.h"
#include "ShaderLoader.h"



BodyRings::BodyRings(RingsData&& inRingsData) : SceneEntity(inRingsData.bodyName + "Rings"),
ringsData(inRingsData), model(ringsData.modelPath, ringsData.opacity < 0.5f ? ShaderLookUpID::Enum::INFRARED_BODY_RINGS : ShaderLookUpID::Enum::VISIBLE_BODY_RINGS), bodyName(ringsData.bodyName)
{

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

	const BlinnPhongMaterial& modelMaterial = model.GetMaterials()[0];
	const Shader& shader = modelMaterial.GetShader();
	shader.Enable();

	renderer.SetModelMatrixVUniform(shader, modelMatrix);

	modelMaterial.EnableTextures();
	model.Render(renderer);
	modelMaterial.DisableTextures();

	shader.Disable();
}