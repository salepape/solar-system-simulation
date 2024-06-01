#include "Orbit.h"

#include <glm/ext/matrix_transform.hpp>

#include "Material.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Utils.h"



// @todo - Build a proper material for the orbits, e.g. a foggy thin tube
Orbit::Orbit(const std::string& texturePath, const float radius) :
	SceneEntity(Material(ResourceLoader::GetShader("Orbit"))), circle({ texturePath, radius })
{
	name = "Orbit";
	bodyName = ResourceLoader::GetNameFromTexturePath(texturePath);
}

void Orbit::ComputeModelMatrixUniform(const float /*elapsedTime*/)
{
	modelMatrix = glm::mat4(1.0f);

	// Cache all needed values to avoid calling ResourceLoader vector search each frame
	if (bodyID == -1)
	{
		const auto& body = ResourceLoader::GetBody(bodyName);
		bodyID = body.ID;
		parentBodyID = body.parentID;
		bodyPreComputations = std::make_unique<PreComputations>(body.preComputations);
	}

	// Center the orbit (non-constant over time) around the parent planet for satellites
	if (parentBodyID != -1)
	{
		modelMatrix = glm::translate(modelMatrix, ResourceLoader::GetBody(parentBodyID).GetPosition());
	}

	// Rotate the orbit (constant over time) around axis colinear to orbit direction to reproduce the orbital plane
	modelMatrix = glm::rotate(modelMatrix, bodyPreComputations->orbInclinationInRad, Utils::forwardVector);
}

void Orbit::Render(const Renderer& renderer, const float /*elapsedTime*/)
{
	ComputeModelMatrixUniform();

	Shader& shader = material.GetShader();

	shader.Enable();
	material.SetDiffuseSamplerFUniform();
	SetModelMatrixUniform(modelMatrix);
	circle.Render(renderer, material.GetDiffuseTextureUnit());
	shader.Disable();
}