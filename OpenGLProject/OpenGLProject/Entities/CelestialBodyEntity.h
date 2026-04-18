#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include <glm/vec3.hpp>

#include <filesystem>
#include <memory>
#include <string>

#include "Components/Meshes/SphereMeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "SceneEntity.h"

class LightSourceComponent;



struct BodyData
{
	std::filesystem::path texturePath;		// DDS texture path
	std::string name;						// Name of the celestial body

	float radius{ 0.0f };					// Planet or moon (only those > pluto radius in length) radius divided by earth's radius [in kms]
	float distanceToParent{ 0.0f };			// Distance between planet (resp. moon) and sun (resp. the planet around which they gravitate) divided by sun-earth distance (= 1AU) [in kms]
	float obliquity{ 0.0f };				// Or axial tilt: angle between planet/moon axis rotation and the normal of its orbital plane [in degrees]
	float orbitalPeriod{ 0.0f };			// Time (sideral) the planet (resp. moon) takes to do one revolution around the sun (resp. its planet) [in Earth days]
	float spinPeriod{ 0.0f };				// Time (sideral) the planet takes to do a rotation on itself [in Earth days]	
	float orbitalInclination{ 0.0f };		// Or "orbital tilt": angle between planet (resp. moon) orbit and the ecliptic [in degrees]

	std::string parentName;					// Name of the celestial body parent, if any
};

// Represent a spherical mesh body, e.g. a planet, a dwarf planet or a moon
class CelestialBodyEntity : public SceneEntity
{
public:
	// Default constructor (not needed)
	CelestialBodyEntity() = delete;

	// User-defined constructor (to be used when building a CelestialBody in-place from initialisation-list)
	CelestialBodyEntity(BodyData&& inBodyData);

	// Copy constructor (needed when we call GetCelestialBody() from BodySystem, as it gets a shallow copy of an instance of this class)
	CelestialBodyEntity(const CelestialBodyEntity& inCelestialBody) = default;
	CelestialBodyEntity& operator = (const CelestialBodyEntity& inCelestialBody) = delete;

	// Move constructor (needed when building a CelestialBody in-place from initialisation-list)
	CelestialBodyEntity(CelestialBodyEntity&& inCelestialBody) = default;
	CelestialBodyEntity& operator = (CelestialBodyEntity&& inCelestialBody) = delete;

	// Destructor (not virtual needed, until child classes exist)
	~CelestialBodyEntity() = default;

	const BodyData& GetBodyData() const { return bodyData; }

	// Compute body position in Cartesian coordinates from Spherical ones
	void ComputeCartesianPosition(const float elapsedTime, const CelestialBodyEntity* satelliteParentBody = nullptr);
	const glm::vec3& GetPosition() const { return position; }

	void Render(const float elapsedTime = 0.0f) override;

private:
	BodyData bodyData;

	SphereMeshComponent sphere;
	BlinnPhongMaterial material;

	std::shared_ptr<LightSourceComponent> lightSource;

	bool isMoon{ false };

	glm::vec3 position{ 0.0f };

	// Angle travelled by the planet (resp. moon) around the sun (resp. planet) since the simulation started [in radians]
	float travelledAngle{ 0.0f };

	// Angular frequency for orbital motion [in radians/Earth days]
	float orbitAngularFreq{ 0.0f };

	// Angular frequency for spin motion [in radians/Earth days]
	float spinAngularFreq{ 0.0f };

	// Obliquity converted [in radians]
	float obliquityInRad{ 0.0f };

	float distCosOrbInclination{ 0.0f };
	float distSinOrbInclination{ 0.0f };

	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& inBodyTexturePath, const std::string& inBodyName);

	void ComputeModelMatrixVUniform(const float elapsedTime = 1.0f) override;
};



#endif // CELESTIAL_BODY_H