#include "BodyRingsEntity.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "Cameras/Camera.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderLoader.h"
#include "Utils/Constants.h"



BodyRingsEntity::BodyRingsEntity(RingsData&& inRingsData) :
	SceneEntity(inRingsData.bodyParent + "Rings"),
	ringsData(inRingsData),
	model(ringsData.modelPath, ShaderLookUpID::Enum::DEFAULT),
	bodyParent(ringsData.bodyParent)
{

}

void BodyRingsEntity::ComputeModelMatrixVUniform(const float /*deltaTime*/, const Camera& /*camera*/, std::optional<std::reference_wrapper<const SceneEntity>> parentEntity)
{
	modelMatrix = parentEntity.value().get().GetModelMatrix();

	// @todo - Make this model scaling work after it has been set in 3D modeling software
	// modelMatrix = glm::scale(modelMatrix, glm::vec3(ringsData.radius));

	// Rotate back (constant over time) around axis normal to orbital plane
	modelMatrix = glm::rotate(modelMatrix, -GLMConstants::halfPi, GLMConstants::rightVector);
}

void BodyRingsEntity::Render()
{
	const BlinnPhongMaterial& modelMaterial = model.GetMaterials()[0];
	const Shader& shader = modelMaterial.GetShader();
	shader.Enable();

	Renderer::SetModelMatrixVUniform(shader, modelMatrix);

	modelMaterial.EnableTextures();
	model.Render();
	modelMaterial.DisableTextures();

	shader.Disable();
}
