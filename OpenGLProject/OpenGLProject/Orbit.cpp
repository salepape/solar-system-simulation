#include "Orbit.h"

#include <glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <utility>

#include "CelestialBody.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Texture.h"
#include "Utils.h"



Orbit::Orbit(BodyData&& inBodyData) : SceneEntity(InitialiseParent(inBodyData.texturePath)),
circle({ inBodyData.distanceToParent })
{
	bodyName = ResourceLoader::GetNameFromTexturePath(inBodyData.texturePath);
	name = bodyName + "Orbit";

	parentBodyID = inBodyData.parentID;

	orbInclinationInRad = glm::radians(inBodyData.orbitalInclination);
}

Material Orbit::InitialiseParent(const std::filesystem::path& inTexturePath)
{
	Texture texture(inTexturePath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, TextureType::DIFFUSE);
	texture.LoadDDS();

	return Material(ResourceLoader::GetShader("Orbit"), { std::move(texture) });
}

void Orbit::ComputeModelMatrixVUniform(const float /*elapsedTime*/)
{
	modelMatrix = glm::mat4(1.0f);

	// Center the orbit (non-constant over time) around the parent planet for satellites
	if (parentBodyID != -1)
	{
		modelMatrix = glm::translate(modelMatrix, ResourceLoader::GetBodySystem(parentBodyID).celestialBody.GetPosition());
	}

	// Rotate the orbit (constant over time) around axis colinear to orbit direction to reproduce the orbital plane
	modelMatrix = glm::rotate(modelMatrix, orbInclinationInRad, Utils::forwardVector);
}

void Orbit::Render(const Renderer& renderer, const float /*elapsedTime*/)
{
	ComputeModelMatrixVUniform();

	Shader& shader = material.GetShader();
	shader.Enable();

	SetModelMatrixVUniform(modelMatrix);

	material.SetDiffuseSamplerFUniform();

	material.EnableTextures();
	circle.Render(renderer);
	material.DisableTextures();

	shader.Disable();
}