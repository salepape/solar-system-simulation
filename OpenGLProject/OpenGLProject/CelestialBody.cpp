#include "CelestialBody.h"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/mat4x4.hpp>
#include <utility>

#include "BodySystem.h"
#include "Constants.h"
#include "Renderer.h"
#include "Shader.h"
#include "ShaderLoader.h"
#include "Texture.h"



CelestialBody::CelestialBody(BodyData&& inBodyData) : SceneEntity(inBodyData.name, InitialiseParent(inBodyData.texturePath, inBodyData.name)),
bodyData(inBodyData),
sphere({ inBodyData.radius })
{
	isMoon = inBodyData.parentName.length() != 0 ? true : false;

	orbitAngularFreq = inBodyData.orbitalPeriod == 0.0f ? 0.0f : GLMConstants::doublePi * 1.0f / inBodyData.orbitalPeriod;
	spinAngularFreq = inBodyData.spinPeriod == 0.0f ? 0.0f : GLMConstants::doublePi * 1.0f / inBodyData.spinPeriod;

	obliquityInRad = glm::radians(inBodyData.obliquity);

	const float orbitalInclinationInRad = glm::radians(inBodyData.orbitalInclination);
	distCosOrbInclination = inBodyData.distanceToParent * glm::cos(orbitalInclinationInRad);
	distSinOrbInclination = inBodyData.distanceToParent * glm::sin(orbitalInclinationInRad);
}

Material CelestialBody::InitialiseParent(const std::filesystem::path& inBodyTexturePath, const std::string& inBodyName)
{
	Texture texture(inBodyTexturePath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, TextureType::Enum::DIFFUSE);
	texture.LoadDDS();

	if (inBodyName == "Sun")
	{
		return Material(ShaderLookUpID::Enum::SUN, { std::move(texture) }, { 0, glm::vec3(1.5f) });
	}
	else
	{
		return Material(ShaderLookUpID::Enum::CELESTIAL_BODY, { std::move(texture) });
	}
}

void CelestialBody::ComputeModelMatrixVUniform(const float elapsedTime)
{
	modelMatrix = glm::mat4(1.0f);

	// Move the body (non-constant over time) according to the circular translations computed previously
	modelMatrix = glm::translate(modelMatrix, position);

	// Rotate the body (constant over time) around an axis colinear to orbit direction to reproduce its axial tilt
	modelMatrix = glm::rotate(modelMatrix, obliquityInRad, GLMConstants::forwardVector);

	// Angle travelled by the celestial body around itself since the simulation started [in radians]
	const float travelledSpinAngle = spinAngularFreq * elapsedTime;

	// Rotate the body (non-constant over time) around axis normal to orbital plane to reproduce its spin
	modelMatrix = glm::rotate(modelMatrix, travelledSpinAngle, GLMConstants::upVector);

	// Rotate the body (constant over time) around axis collinear to orbital plane so its poles appear vertically
	modelMatrix = glm::rotate(modelMatrix, GLMConstants::halfPi, GLMConstants::rightVector);
}

void CelestialBody::ComputeCartesianPosition(const float elapsedTime, const CelestialBody* satelliteParentBody)
{
	position = glm::vec3(0.0f);

	// Angle travelled by the planet (resp. moon) around the sun (resp. planet) since the simulation started [in radians]
	const float travelledOrbitAngle = orbitAngularFreq * elapsedTime;
	if (isMoon == false || satelliteParentBody == nullptr)
	{
		travelledAngle = travelledOrbitAngle;
	}
	// Circular translation of satellite around corresponding planet, taking into account satellite "orbital tilt"
	else
	{
		const float sinTravelledAngleParent = glm::sin(satelliteParentBody->travelledAngle);

		position += glm::vec3(
			satelliteParentBody->distCosOrbInclination * sinTravelledAngleParent,
			satelliteParentBody->distSinOrbInclination * sinTravelledAngleParent,
			satelliteParentBody->bodyData.distanceToParent * glm::cos(satelliteParentBody->travelledAngle));
	}

	// Circular translation of body around Sun, taking into account body "orbital tilt"
	const float sinTravelledAngle = glm::sin(travelledOrbitAngle);

	position += glm::vec3(
		distCosOrbInclination * sinTravelledAngle,
		distSinOrbInclination * sinTravelledAngle,
		bodyData.distanceToParent * glm::cos(travelledOrbitAngle));
}

void CelestialBody::Render(const Renderer& renderer, const float elapsedTime)
{
	ComputeModelMatrixVUniform(elapsedTime);

	Shader& shader = material.GetShader();
	shader.Enable();

	SetModelMatrixVUniform();

	material.EnableTextures();
	sphere.Render(renderer);
	material.EnableTextures();

	shader.Disable();
}