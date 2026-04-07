#include "BodyRings.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "Constants.h"
#include "Renderer.h"
#include "Shader.h"
#include "ShaderLoader.h"



BodyRings::BodyRings(RingsData&& inRingsData) : SceneEntity(inRingsData.bodyName + "Rings", InitialiseParent(inRingsData.opacity)),
ringsData(inRingsData), model(inRingsData.modelPath), bodyName(ringsData.bodyName)
{
	// @todo - Find a way not to have to initialise a Material instance as part of the SceneEntity construction
	// Done as part of the Model construction, apart from ShaderLookUpID reference
}

BlinnPhongMaterial BodyRings::InitialiseParent(const float inRingsOpacity)
{
	if (inRingsOpacity < 0.5f)
	{
		return BlinnPhongMaterial(ShaderLookUpID::Enum::INFRARED_BODY_RINGS, { /* texturesLoadedFromTheModel */ });
	}
	else
	{
		return BlinnPhongMaterial(ShaderLookUpID::Enum::VISIBLE_BODY_RINGS, { /* texturesLoadedFromTheModel */ });
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