#include "CelestialBody.h"

#include <glm/trigonometric.hpp>

#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Utils.h"



CelestialBody::CelestialBody(const std::string& inTexturePath, const float inRadius, const float inDistanceToParent, const float inObliquity, const float inOrbitalPeriod, const float inSpinPeriod, const float inOrbitalInclination, const int32_t inParentID) :
	texturePath(inTexturePath), radius(inRadius), distanceToParent(inDistanceToParent), obliquity(inObliquity), orbitalPeriod(inOrbitalPeriod), spinPeriod(inSpinPeriod), orbitalInclination(inOrbitalInclination), parentID(inParentID),
	SceneEntity(InitialiseParent(inTexturePath)), sphere({ inTexturePath, inRadius }), orbit({ inTexturePath, inDistanceToParent }), preComputations(LoadPreComputations())
{
	// @todo - Find a way to avoid building this string again
	name = ResourceLoader::GetNameFromTexturePath(inTexturePath);

	if (name == "Sun")
	{
		directionalLight = std::make_unique<DirectionalLight>();
		directionalLight->Store({ material.GetShader().GetRendererID() });
	}
	else
	{
		pointLight = std::make_unique<PointLight>();
		pointLight->Store({ material.GetShader().GetRendererID() });
	}
}

Material CelestialBody::InitialiseParent(const std::string& inTexturePath)
{
	const std::string& bodyNameLocal = ResourceLoader::GetNameFromTexturePath(inTexturePath);
	if (bodyNameLocal == "Sun")
	{
		return Material(ResourceLoader::GetShader("Sun"), { 1.0f, 0.0f, 0.0f }, 95.0f);
	}
	else
	{
		return Material(ResourceLoader::GetShader("CelestialBody"));
	}
}

PreComputations CelestialBody::LoadPreComputations()
{
	const float orbitalInclinationInRad = glm::radians(orbitalInclination);

	float textHeightFactor = 1.5f;
	float textScaleFactor = 0.01f;
	if (name == "Sun")
	{
		textHeightFactor = 1.25f;
		textScaleFactor = 0.0075f;
	}
	// If the current celestial body is a satellite (i.e. has a parent)
	else if (parentID != -1)
	{
		textHeightFactor = 3.5f;
		textScaleFactor = 0.03f;
	}

	return {
		orbitalPeriod == 0.0f ? 0.0f : Utils::doublePi * 1.0f / orbitalPeriod,
		spinPeriod == 0.0f ? 0.0f : Utils::doublePi * 1.0f / spinPeriod,

		glm::radians(obliquity),
		orbitalInclinationInRad,

		distanceToParent * glm::cos(orbitalInclinationInRad),
		distanceToParent * glm::sin(orbitalInclinationInRad),

		radius * textHeightFactor,
		radius * textScaleFactor
	};
}

void CelestialBody::Render(const Renderer& renderer, uint32_t& textureUnit, const glm::mat4& modelMatrix)
{
	Shader& shader = material.GetShader();

	shader.Enable();
	SetModelMatrixUniform(modelMatrix);
	material.SetDiffuseSamplerVUniform(textureUnit);
	sphere.Render(renderer, textureUnit++);
	shader.Disable();
}