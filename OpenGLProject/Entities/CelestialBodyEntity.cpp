#include "CelestialBodyEntity.h"

#include <glad/glad.h>
#include <glm/trigonometric.hpp>

#include <utility>
#include <vector>

#include "Application/Application.h"
#include "Cameras/Camera.h"
#include "Components/Lights/LightSourceComponent.h"
#include "Components/Lights/PointLightComponent.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderLoader.h"
#include "Rendering/Texture.h"
#include "Utils/Constants.h"



CelestialBodyEntity::CelestialBodyEntity(const BodyData& inBodyData) :
	SceneEntity(inBodyData.name),
	bodyData(std::move(inBodyData)),
	sphere(bodyData.radius),
	material(InitialiseMaterial(bodyData.texturePath))
{
	if (bodyData.type == "Star")
	{
		// Set up the lighting for all Scene Entities according to Star position/light emission parameters
		lightSource = std::make_unique<PointLightComponent>(GetPosition(),
			ReflectionParams{ glm::vec3(0.25f), glm::vec3(0.95f), glm::vec3(0.0f) },
			AttenuationParams{ 1.0f, 0.00045f, 0.00000075f });
	}

	orbitAngularFreq = bodyData.orbitalPeriod == 0.0f ? 0.0f : GLMConstants::doublePi * 1.0f / bodyData.orbitalPeriod;
	spinAngularFreq = bodyData.spinPeriod == 0.0f ? 0.0f : GLMConstants::doublePi * 1.0f / bodyData.spinPeriod;

	obliquityInRad = glm::radians(bodyData.obliquity);

	const float orbitalInclinationInRad = glm::radians(bodyData.orbitalInclination);
	distCosOrbInclination = bodyData.distanceToParent * glm::cos(orbitalInclinationInRad);
	distSinOrbInclination = bodyData.distanceToParent * glm::sin(orbitalInclinationInRad);
}

BlinnPhongMaterial CelestialBodyEntity::InitialiseMaterial(const std::filesystem::path& texturePath)
{
	Texture texture(texturePath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, TextureType::Enum::DIFFUSE);
	texture.LoadDDS();

	if (bodyData.type == "Star")
	{
		// Allow the Star texture to be rendered with higher intensity than a simple white light - give volcanic visual effect)
		constexpr float oversaturatingFactor = 1.5f;
		return BlinnPhongMaterial(ShaderLookUpID::Enum::STAR, std::vector<Texture>{ std::move(texture) }, DiffuseProperties{ GLMConstants::whiteColour * oversaturatingFactor });
	}
	else
	{
		return BlinnPhongMaterial(ShaderLookUpID::Enum::DEFAULT, std::vector<Texture>{ std::move(texture) });
	}
}

void CelestialBodyEntity::ComputeTransformVUniform(const float deltaTime, const Camera& /*camera*/, std::optional<std::reference_wrapper<const ITransformable>> parentTransformable)
{
	// @todo - Can we avoid redoing the computation all time from scratch and just add delta transform?
	transform.Reset();

	const float alteredDeltaTime = deltaTime * Application::GetInstance().GetSpeedFactor();

	ComputeCartesianPosition(alteredDeltaTime, parentTransformable);

	transform.Translate(position);

	// Rotate the body (constant over time) around an axis colinear to orbital plane and tangent to orbital trajectory to reproduce its axial tilt
	transform.Rotate(obliquityInRad, WorldSpace::ZUnitVector);

	// Angle travelled by the celestial body around itself for the current frame [in radians]
	travelledSpinAngle += spinAngularFreq * alteredDeltaTime;

	// Rotate the body (non-constant over time) around axis normal to orbital plane to reproduce its spin
	transform.Rotate(travelledSpinAngle, WorldSpace::YUnitVector);

	// Rotate the body (constant over time) around axis colinear to orbital plane and normal to orbital trajectory so its poles appear vertically
	transform.Rotate(GLMConstants::halfPi, WorldSpace::XUnitVector);
}

void CelestialBodyEntity::ComputeCartesianPosition(const float deltaTime, std::optional<std::reference_wrapper<const ITransformable>> parentTransformable)
{
	position = glm::vec3(0.0f);

	// Angle travelled by Planet (resp. Moon) around Star (resp. Planet) since the simulation started [in radians]
	travelledOrbitAngle += orbitAngularFreq * deltaTime;
	if (parentID == 0 || parentTransformable.has_value() == false)
	{
		travelledAngle = travelledOrbitAngle;
	}
	// Circular translation of satellite around corresponding planet, taking into account satellite "orbital tilt"
	else
	{
		// Warning: assumes body pointer is valid!
		const CelestialBodyEntity& satelliteParentBodyRef = *dynamic_cast<const CelestialBodyEntity*>(&parentTransformable.value().get());

		const float sinTravelledAngleParent = glm::sin(satelliteParentBodyRef.travelledAngle);

		position += glm::vec3(
			satelliteParentBodyRef.distCosOrbInclination * sinTravelledAngleParent,
			satelliteParentBodyRef.distSinOrbInclination * sinTravelledAngleParent,
			satelliteParentBodyRef.bodyData.distanceToParent * glm::cos(satelliteParentBodyRef.travelledAngle));
	}

	// Circular translation of body around Star, taking into account body "orbital tilt"
	const float sinTravelledAngle = glm::sin(travelledOrbitAngle);

	position += glm::vec3(
		distCosOrbInclination * sinTravelledAngle,
		distSinOrbInclination * sinTravelledAngle,
		bodyData.distanceToParent * glm::cos(travelledOrbitAngle));
}

void CelestialBodyEntity::Render()
{
	const Shader& shader = material.GetShader();
	shader.Enable();

	Renderer::SetTransformVUniform(shader, transform);

	material.EnableTextures();
	sphere.Render();
	material.EnableTextures();

	shader.Disable();
}
