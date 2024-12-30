#include "CelestialBody.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/mat4x4.hpp>
#include <utility>

#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Texture.h"
#include "Utils.h"



// @todo - Find a way to avoid building this string (GetNameFromTexturePath method) 3 times
CelestialBody::CelestialBody(BodyData&& inBodyData) : SceneEntity(InitialiseParent(inBodyData.texturePath)),
bodyData(inBodyData),
sphere({ inBodyData.radius })
{
	name = ResourceLoader::GetNameFromTexturePath(inBodyData.texturePath);

	orbitAngularFreq = inBodyData.orbitalPeriod == 0.0f ? 0.0f : Utils::doublePi * 1.0f / inBodyData.orbitalPeriod;
	spinAngularFreq = inBodyData.spinPeriod == 0.0f ? 0.0f : Utils::doublePi * 1.0f / inBodyData.spinPeriod;

	obliquityInRad = glm::radians(inBodyData.obliquity);

	const float orbitalInclinationInRad = glm::radians(inBodyData.orbitalInclination);
	distCosOrbInclination = inBodyData.distanceToParent * glm::cos(orbitalInclinationInRad);
	distSinOrbInclination = inBodyData.distanceToParent * glm::sin(orbitalInclinationInRad);
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

void CelestialBody::ComputeModelMatrixVUniform(const float elapsedTime)
{
	modelMatrix = glm::mat4(1.0f);

	// Move the body (non-constant over time) according to the circular translations computed previously
	modelMatrix = glm::translate(modelMatrix, position);

	// Rotate the body (constant over time) around an axis colinear to orbit direction to reproduce its axial tilt
	modelMatrix = glm::rotate(modelMatrix, obliquityInRad, Utils::forwardVector);

	// Angle travelled by the celestial body around itself since the simulation started [in radians]
	const float travelledSpinAngle = spinAngularFreq * elapsedTime;

	// Rotate the body (non-constant over time) around axis normal to orbital plane to reproduce its spin
	modelMatrix = glm::rotate(modelMatrix, travelledSpinAngle, Utils::upVector);

	// Rotate the body (constant over time) around axis colinear to orbital plane so its poles appear vertically
	modelMatrix = glm::rotate(modelMatrix, Utils::halfPi, Utils::rightVector);
}

void CelestialBody::ComputeCartesianPosition(const float elapsedTime)
{
	position = glm::vec3(0.0f);

	// Angle travelled by the planet (resp. moon) around the sun (resp. planet) since the simulation started [in radians]
	const float travelledOrbitAngle = orbitAngularFreq * elapsedTime;
	if (bodyData.parentID == -1)
	{
		travelledAngle = travelledOrbitAngle;
	}
	// Circular translation of satellite around corresponding planet, taking into account satellite "orbital tilt"
	else
	{
		const CelestialBody& satelliteParentBody = ResourceLoader::GetBodySystem(bodyData.parentID).celestialBody;
		const float sinTravelledAngleParent = glm::sin(satelliteParentBody.travelledAngle);

		position += glm::vec3(
			satelliteParentBody.distCosOrbInclination * sinTravelledAngleParent,
			satelliteParentBody.distSinOrbInclination * sinTravelledAngleParent,
			satelliteParentBody.bodyData.distanceToParent * glm::cos(satelliteParentBody.travelledAngle));
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
}