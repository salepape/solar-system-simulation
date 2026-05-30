#include "OrbitEntity.h"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include <utility>
#include <vector>

#include "Cameras/Camera.h"
#include "CelestialBodyEntity.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderLoader.h"
#include "Rendering/Texture.h"
#include "Utils/Constants.h"



OrbitEntity::OrbitEntity(const BodyData& inBodyData) :
	SceneEntity(inBodyData.name + "Orbit"),
	circle(inBodyData.distanceToParent),
	material(InitialiseMaterial(inBodyData.texturePath))
{
	orbInclinationInRad = glm::radians(inBodyData.orbitalInclination);
}

BlinnPhongMaterial OrbitEntity::InitialiseMaterial(const std::filesystem::path& texturePath)
{
	Texture texture(texturePath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, TextureType::Enum::DIFFUSE);
	texture.LoadDDS();

	return BlinnPhongMaterial(ShaderLookUpID::Enum::DEFAULT, std::vector<Texture>{ std::move(texture) });
}

void OrbitEntity::ComputeModelMatrixVUniform(const float /*deltaTime*/, const Camera& /*camera*/, std::optional<std::reference_wrapper<const SceneEntity>> parentEntity)
{
	modelMatrix = glm::mat4(1.0f);

	// Center the orbit (non-constant over time) around the parent planet for satellites
	// Only moons have their parent position (= Planet) moving, whereas planets have their parent position (= Sun) constant
	if (parentID != 0)
	{
		// @todo - Last argument could be abstracted in Transform object with a GetPosition() method. Same for rotation and scale
		modelMatrix = glm::translate(modelMatrix, glm::vec3(parentEntity.value().get().GetModelMatrix()[3]));
	}

	// Rotate the orbit (constant over time) around axis colinear to orbit direction to reproduce the orbital plane
	modelMatrix = glm::rotate(modelMatrix, orbInclinationInRad, GLMConstants::forwardVector);
}

void OrbitEntity::Render()
{
	const Shader& shader = material.GetShader();
	shader.Enable();

	Renderer::SetModelMatrixVUniform(shader, modelMatrix);

	material.EnableTextures();
	circle.Render();
	material.DisableTextures();

	shader.Disable();
}
