#include "BodyRingsEntity.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "Rendering/BlinnPhongMaterial.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderLoader.h"
#include "Utils/Constants.h"



BodyRingsEntity::BodyRingsEntity(RingsData&& inRingsData) : SceneEntity(inRingsData.bodyName + "Rings"),
ringsData(inRingsData), model(ringsData.modelPath, ShaderLookUpID::Enum::DEFAULT), bodyName(ringsData.bodyName)
{

}

void BodyRingsEntity::ComputeModelMatrixVUniform(const glm::mat4& inModelMatrix, const float /*elapsedTime*/)
{
	modelMatrix = inModelMatrix;

	// @todo - Make this model scaling work if possible
	// modelMatrix = glm::scale(modelMatrix, glm::vec3(ringsData.radius));

	// Rotate back (constant over time) around axis normal to orbital plane
	modelMatrix = glm::rotate(modelMatrix, -GLMConstants::halfPi, GLMConstants::rightVector);
}

void BodyRingsEntity::Render(const Renderer& renderer, const glm::mat4& inModelMatrix, const float /*elapsedTime*/)
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