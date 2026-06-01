#include "OrbitEntity.h"

#include <glad/glad.h>
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
#include "Scene/Transform.h"
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

void OrbitEntity::ComputeTransformVUniform(const float /*deltaTime*/, const Camera& /*camera*/, std::optional<std::reference_wrapper<const SceneEntity>> parentEntity)
{
	transform.Reset();

	// Center the orbit (non-constant over time) around the parent planet for satellites
	// Only moons have their parent position (= Planet) moving, whereas planets have their parent position (= Sun) constant
	if (parentID != 0)
	{
		transform.Translate(parentEntity.value().get().GetTransform().GetPosition());
	}

	// Rotate the orbit (constant over time) around axis colinear to orbital plane + tangent to orbital trajectory to reproduce its axial tilt?
	transform.Rotate(orbInclinationInRad, WorldSpace::ZUnitVector);
}

void OrbitEntity::Render()
{
	const Shader& shader = material.GetShader();
	shader.Enable();

	Renderer::SetTransformVUniform(shader, transform);

	material.EnableTextures();
	circle.Render();
	material.DisableTextures();

	shader.Disable();
}
