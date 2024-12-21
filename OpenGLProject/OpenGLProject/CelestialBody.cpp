#include "CelestialBody.h"

#include <glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/mat4x4.hpp>
#include <utility>

#include "BodyRings.h"
#include "PointLight.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Texture.h"
#include "Utils.h"



// @todo - Find a way to avoid building this string (GetNameFromTexturePath method) 3 times
CelestialBody::CelestialBody(BodyData&& inBodyData) : SceneEntity(InitialiseParent(inBodyData.texturePath)),
bodyData(inBodyData),
sphere({ inBodyData.radius }),
orbit({ inBodyData.texturePath, bodyData.distanceToParent }),
billboard({ ResourceLoader::GetNameFromTexturePath(inBodyData.texturePath) }),
preComputations(LoadPreComputations())
{
	// Compute position first so the point light is correctly initialised in the case of the Sun
	ComputeCartesianPosition(1.0f);

	name = ResourceLoader::GetNameFromTexturePath(inBodyData.texturePath);
	if (name == "Sun")
	{
		// Set up the lighting for all scene entities according to Sun position/light emission parameters
		lightSource = std::make_unique<PointLight>(position,
			ReflectionParams{ glm::vec3(0.25f), glm::vec3(0.95f), glm::vec3(1.0f) },
			AttenuationParams{ 1.0f, 0.00045f, 0.00000075f });
	}

	// Attach a ring system to the celestial body if one
	if (bodyData.hasRings)
	{
		bodyRings = std::make_unique<BodyRings>(std::move(ResourceLoader::GetBodyRings(name)));
	}
}

Material CelestialBody::InitialiseParent(const std::filesystem::path& inTexturePath)
{
	Texture texture(inTexturePath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, TextureType::DIFFUSE);
	texture.LoadDDS();

	if (const std::string& bodyName = ResourceLoader::GetNameFromTexturePath(inTexturePath);
		bodyName == "Sun")
	{
		return Material(ResourceLoader::GetShader("Sun"), { std::move(texture) }, { 0, glm::vec3(1.5f) });
	}
	else
	{
		return Material(ResourceLoader::GetShader("CelestialBody"), { std::move(texture) });
	}
}

PreComputations CelestialBody::LoadPreComputations()
{
	const float orbitalInclinationInRad = glm::radians(bodyData.orbitalInclination);

	float textHeightFactor = 1.5f;
	float textScaleFactor = 0.01f;
	if (name == "Sun")
	{
		textHeightFactor = 1.25f;
		textScaleFactor = 0.0075f;
	}
	// If the current celestial body is a satellite (i.e. has a parent)
	else if (bodyData.parentID != -1)
	{
		textHeightFactor = 3.5f;
		textScaleFactor = 0.03f;
	}

	return {
		bodyData.orbitalPeriod == 0.0f ? 0.0f : Utils::doublePi * 1.0f / bodyData.orbitalPeriod,
		bodyData.spinPeriod == 0.0f ? 0.0f : Utils::doublePi * 1.0f / bodyData.spinPeriod,

		glm::radians(bodyData.obliquity),
		orbitalInclinationInRad,

		bodyData.distanceToParent * glm::cos(orbitalInclinationInRad),
		bodyData.distanceToParent * glm::sin(orbitalInclinationInRad),

		bodyData.radius * textHeightFactor,
		bodyData.radius * textScaleFactor
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
	position = glm::vec3(0.0f);

	// Angle travelled by the planet (resp. moon) around the sun (resp. planet) since the simulation started [in radians]
	const float travelledOrbitAngle = preComputations.orbitAngularFreq * elapsedTime;
	if (bodyData.parentID == -1)
	{
		travelledAngle = travelledOrbitAngle;
	}
	// Circular translation of satellite around corresponding planet, taking into account satellite "orbital tilt"
	else
	{
		const CelestialBody& satelliteParent = ResourceLoader::GetBody(bodyData.parentID);

		const float sinTravelledAngleParent = glm::sin(satelliteParent.travelledAngle);

		position += glm::vec3(
			satelliteParent.preComputations.distCosOrbInclination * sinTravelledAngleParent,
			satelliteParent.preComputations.distSinOrbInclination * sinTravelledAngleParent,
			satelliteParent.bodyData.distanceToParent * glm::cos(satelliteParent.travelledAngle));
	}

	// Circular translation of body around Sun, taking into account body "orbital tilt"
	const float sinTravelledAngle = glm::sin(travelledOrbitAngle);

	position += glm::vec3(
		preComputations.distCosOrbInclination * sinTravelledAngle,
		preComputations.distSinOrbInclination * sinTravelledAngle,
		bodyData.distanceToParent * glm::cos(travelledOrbitAngle));
}

void CelestialBody::Render(const Renderer& renderer, const float elapsedTime)
{
	// Update material uniforms and draw sphere
	ComputeModelMatrixVUniform(elapsedTime);

	Shader& shader = material.GetShader();
	shader.Enable();

	SetModelMatrixVUniform(modelMatrix);

	material.SetDiffuseSamplerFUniform();

	material.EnableTextures();
	sphere.Render(renderer);
	material.EnableTextures();

	shader.Disable();

	// Draw semi-transparent Saturn rings
	if (bodyRings)
	{
		bodyRings->Render(renderer);
	}

	// Draw orbit
	orbit.Render(renderer);
}