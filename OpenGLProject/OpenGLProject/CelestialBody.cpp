#include "CelestialBody.h"

#include <glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/mat4x4.hpp>
#include <utility>

#include "DirectionalLight.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "PointLight.h"
#include "Texture.h"
#include "Utils.h"



// @todo - Find a way to avoid building this string (GetNameFromTexturePath method) 3 times 
CelestialBody::CelestialBody(const std::string& inTexturePath, const float inRadius, const float inDistanceToParent, const float inObliquity, const float inOrbitalPeriod, const float inSpinPeriod, const float inOrbitalInclination, const int32_t inParentID) : SceneEntity(InitialiseParent(inTexturePath)),
texturePath(inTexturePath), radius(inRadius), distanceToParent(inDistanceToParent), obliquity(inObliquity), orbitalPeriod(inOrbitalPeriod), spinPeriod(inSpinPeriod), orbitalInclination(inOrbitalInclination), parentID(inParentID),
sphere({ inRadius }),
orbit({ inTexturePath, inDistanceToParent }),
billboard({ ResourceLoader::GetNameFromTexturePath(inTexturePath) }),
preComputations(LoadPreComputations())
{
	name = ResourceLoader::GetNameFromTexturePath(inTexturePath);
	if (name == "Sun")
	{
		lightSource = std::make_unique<DirectionalLight>(ResourceLoader::GetShader("Sun"));
	}
	else
	{
		lightSource = std::make_unique<PointLight>();
	}

	ComputeCartesianPosition(1.0f);
}

CelestialBody::CelestialBody(CelestialBody&& inCelestialBody) = default;
CelestialBody::~CelestialBody() = default;

Material CelestialBody::InitialiseParent(const std::string& inTexturePath)
{
	Texture texture(inTexturePath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, TextureType::DIFFUSE);
	texture.LoadDDS();

	if (const std::string& bodyName = ResourceLoader::GetNameFromTexturePath(inTexturePath);
		bodyName == "Sun")
	{
		return Material(ResourceLoader::GetShader("Sun"), { std::move(texture) }, { 1.0f, 0.0f, 0.0f }, 95.0f);
	}
	else
	{
		return Material(ResourceLoader::GetShader("CelestialBody"), { std::move(texture) });
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

void CelestialBody::ComputeModelMatrixVUniform(const float elapsedTime)
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

void CelestialBody::Render(const Renderer& renderer, const float elapsedTime)
{
	ComputeModelMatrixVUniform(elapsedTime);

	Shader& shader = material.GetShader();
	shader.Enable();

	SetModelMatrixVUniform(modelMatrix);

	material.SetDiffuseSamplerFUniform();

	material.EnableTextures();
	sphere.Render(renderer);
	material.EnableTextures();

	shader.Disable();
}