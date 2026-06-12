#include "BodyRingsEntity.h"

//#include <glm/vec3.hpp>

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

void BodyRingsEntity::ComputeTransformVUniform(const float /*deltaTime*/, const Camera& /*camera*/, std::optional<std::reference_wrapper<const SceneEntity>> parentEntity)
{
	transform.Set(parentEntity.value().get().GetTransform().Get());

	// @todo - Make this model scaling work after it has been set in 3D modeling software
	// transform.Scale(glm::vec3(ringsData.radius));

	// Rotate (constant over time) around axis colinear to orbital plane and normal to orbital trajectory
	transform.Rotate(-GLMConstants::halfPi, WorldSpace::XUnitVector);
}

void BodyRingsEntity::Render()
{
	const BlinnPhongMaterial& modelMaterial = model.GetMaterials()[0];
	const Shader& shader = modelMaterial.GetShader();
	shader.Enable();

	Renderer::SetTransformVUniform(shader, transform);

	modelMaterial.EnableTextures();
	model.Render();
	modelMaterial.DisableTextures();

	shader.Disable();
}
