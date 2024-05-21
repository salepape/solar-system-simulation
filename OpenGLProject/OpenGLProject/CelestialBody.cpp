#include "CelestialBody.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Utils.h"



CelestialBody::CelestialBody(const std::string& inTexturePath, const float inRadius, const float inDistanceToParent, const float inObliquity, const float inOrbitalPeriod, const float inSpinPeriod, const float inOrbitalInclination, const int32_t inParentID) :
	texturePath(inTexturePath), radius(inRadius), distanceToParent(inDistanceToParent), obliquity(inObliquity), orbitalPeriod(inOrbitalPeriod), spinPeriod(inSpinPeriod), orbitalInclination(inOrbitalInclination), parentID(inParentID),
	SceneEntity(InitialiseParent(inTexturePath)), sphere({ inTexturePath, inRadius }), orbit({ inTexturePath, inDistanceToParent }), preComputations(LoadPreComputations()),
	position(glm::vec3((0.0f, 0.0f, 0.0f)))
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

void CelestialBody::ComputeModelMatrixUniform(const float elapsedTime)
{
	modelMatrix = glm::mat4(1.0f);

	// Move the body (non-constant over time) according to the circular translations computed previously
	modelMatrix = glm::translate(modelMatrix, position);

	// Rotate the body (constant over time) around an axis colinear to orbit direction to reproduce its axial tilt
	modelMatrix = glm::rotate(modelMatrix, preComputations.obliquityInRad, Utils::forwardVector);

	// Angle travelled by the celestial body around itself since the simulation started [in radians]
	const float travelledSpinAngle = preComputations.spinAngularFreq * elapsedTime;

	// Rotate the body (non-constant over time) around axis normal to orbital plane to reproduce its spin
	modelMatrix = glm::rotate(modelMatrix, travelledSpinAngle, Utils::upVector);

	// Rotate the body (constant over time) around axis colinear to orbital plane so its poles appear vertically
	modelMatrix = glm::rotate(modelMatrix, Utils::halfPi, Utils::rightVector);
}

void CelestialBody::Render(const Renderer& renderer, const float elapsedTime)
{
	ComputeModelMatrixUniform(elapsedTime);

	Shader& shader = material.GetShader();

	shader.Enable();
	SetModelMatrixUniform(modelMatrix);
	material.SetDiffuseSamplerVUniform();
	sphere.Render(renderer, material.GetDiffuseSamplerID());
	shader.Disable();
}

void CelestialBody::ComputeCartesianPosition(const float elapsedTime)
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);

	// Angle travelled by the planet (resp. moon) around the sun (resp. planet) since the simulation started [in radians]
	const float travelledOrbitAngle = preComputations.orbitAngularFreq * elapsedTime;
	if (parentID == -1)
	{
		travelledAngle = travelledOrbitAngle;
	}
	// Circular translation of satellite around corresponding planet, taking into account satellite "orbital tilt"
	else
	{
		const auto& satelliteParent = ResourceLoader::GetBody(parentID);

		const float sinTravelledAngleParent = glm::sin(satelliteParent.travelledAngle);

		position += glm::vec3(
			satelliteParent.preComputations.distCosOrbInclination * sinTravelledAngleParent,
			satelliteParent.preComputations.distSinOrbInclination * sinTravelledAngleParent,
			satelliteParent.distanceToParent * glm::cos(satelliteParent.travelledAngle));
	}

	// Circular translation of body around Sun, taking into account body "orbital tilt"
	const float sinTravelledAngle = glm::sin(travelledOrbitAngle);

	position += glm::vec3(
		preComputations.distCosOrbInclination * sinTravelledAngle,
		preComputations.distSinOrbInclination * sinTravelledAngle,
		distanceToParent * glm::cos(travelledOrbitAngle));
}
