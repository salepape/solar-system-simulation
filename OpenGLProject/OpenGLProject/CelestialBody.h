#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include <filesystem>
#include <glm/vec3.hpp>
#include <memory>
#include <string>

#include "Billboard.h"
#include "Material.h"
#include "Orbit.h"
#include "SceneEntity.h"
#include "Sphere.h"

class Renderer;



struct BodyData
{
	std::filesystem::path texturePath;		// DDS texture path

	float radius{ 0.0f };					// Planet or moon (only those > pluto radius in length) radius divided by earth's radius [in kms]
	float distanceToParent{ 0.0f };			// Distance between planet (resp. moon) and sun (resp. the planet around which they gravitate) divided by sun-earth distance (= 1AU) [in kms]
	float obliquity{ 0.0f };				// Or axial tilt: angle between planet/moon axis rotation and the normal of its orbital plane [in degrees]
	float orbitalPeriod{ 0.0f };			// Time (sideral) the planet (resp. moon) takes to do one revolution around the sun (resp. its planet) [in Earth days]
	float spinPeriod{ 0.0f };				// Time (sideral) the planet takes to do a rotation on itself [in Earth days]	
	float orbitalInclination{ 0.0f };		// Or "orbital tilt": angle between planet (resp. moon) orbit and the ecliptic [in degrees]

	bool hasRings{ false };					// Does the celestial body has a ring system around it

	int32_t parentID{ -1 };					// Pointer to the planet mesh around which a moon rotates
};

// Represent a spherical mesh body, e.g. a planet, a dwarf planet or a moon
class CelestialBody : public SceneEntity
{
public:
	CelestialBody(BodyData&& inBodyData);
	CelestialBody(CelestialBody&& inCelestialBody) = default;
	CelestialBody& operator = (CelestialBody&& inCelestialBody) = delete;

	CelestialBody() = delete;
	CelestialBody(const CelestialBody& inCelestialBody) = delete;
	CelestialBody& operator = (const CelestialBody& inCelestialBody) = delete;

	~CelestialBody() = default;

	BodyData bodyData;

	// Compute body position in Cartesian coordinates from Spherical ones
	void ComputeCartesianPosition(const float elapsedTime);
	const glm::vec3& GetPosition() const { return position; }

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	Sphere sphere;

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

	static Material InitialiseParent(const std::filesystem::path& inTexturePath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 1.0f) override;
};



#endif // CELESTIAL_BODY_H