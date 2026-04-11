#include "OrbitEntity.h"

#include <glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <utility>
#include <vector>

#include "CelestialBodyEntity.h"
#include "Constants.h"
#include "Renderer.h"
#include "Shader.h"
#include "ShaderLoader.h"
#include "Texture.h"



OrbitEntity::OrbitEntity(const BodyData& inBodyData) : SceneEntity(inBodyData.name + "Orbit"),
circle(inBodyData.distanceToParent), material(InitialiseMaterial(inBodyData.texturePath))
{
	isMoon = (inBodyData.parentName.length() != 0);

	orbInclinationInRad = glm::radians(inBodyData.orbitalInclination);

	ComputeModelMatrixVUniform(glm::vec3(0.0f));
}

BlinnPhongMaterial OrbitEntity::InitialiseMaterial(const std::filesystem::path& inTexturePath)
{
	Texture texture(inTexturePath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, TextureType::Enum::DIFFUSE);
	texture.LoadDDS();

	return BlinnPhongMaterial(ShaderLookUpID::Enum::ORBIT, std::vector<Texture>{ std::move(texture) });
}

void OrbitEntity::ComputeModelMatrixVUniform(const glm::vec3& parentPosition, const float /*elapsedTime*/)
{
	modelMatrix = glm::mat4(1.0f);

	// @todo - Implement entity attachment to transforms inherited automatically in a 'is-a' composition relationship
	// Center the orbit (non-constant over time) around the parent planet for satellites
	if (isMoon)
	{
		modelMatrix = glm::translate(modelMatrix, parentPosition);
	}

	// Rotate the orbit (constant over time) around axis colinear to orbit direction to reproduce the orbital plane
	modelMatrix = glm::rotate(modelMatrix, orbInclinationInRad, GLMConstants::forwardVector);
}

void OrbitEntity::Render(const Renderer& renderer, const glm::vec3& parentPosition, const float /*elapsedTime*/)
{
	// Only moons have their parent position (= Planet) moving, whereas planets have their parent position (= Sun) constant
	if (isMoon)
	{
		ComputeModelMatrixVUniform(parentPosition);
	}

	const Shader& shader = material.GetShader();
	shader.Enable();

	renderer.SetModelMatrixVUniform(shader, modelMatrix);

	material.EnableTextures();
	circle.Render(renderer);
	material.DisableTextures();

	shader.Disable();
}