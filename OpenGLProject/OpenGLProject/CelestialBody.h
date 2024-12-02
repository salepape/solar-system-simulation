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

class BodyRings;
class LightSource;
class Renderer;



struct BodyData
{
	std::filesystem::path texturePath;		// DDS texture path

	float radius{ 0.0f };					// Planet or moon (only those > pluto radius in length) radius divided by earth's radius [in kms]
	float distanceToParent{ 0.0f };			// Distance between planet (resp. moon) and sun (resp. the planet around which they gravitate) divided by sun-earth distance (= 1AU) [in kms]
	float obliquity{ 0.0f };				// Or axial tilt: angle between planet/moon axis rotation and the normal of its orbital plane [in degrees]
	float orbitalPeriod{ 0.0f };			// Time (sideral) the planet (resp. moon) takes to do one revolution around the sun (resp. its planet) [in earth days]
	float spinPeriod{ 0.0f };				// Time (sideral) the planet takes to do a rotation on itself [in earth days]	
	float orbitalInclination{ 0.0f };		// Or "orbital tilt": angle between planet (resp. moon) orbit and the ecliptic [in degrees]

	int32_t parentID{ -1 };					// Pointer to the planet mesh around which a moon rotates
};

// Represent all Render loop constants of a celestial body
struct PreComputations
{
	float orbitAngularFreq{ 0.0f };			// Angular frequency for orbital motion [in radians/Earth days]
	float spinAngularFreq{ 0.0f };			// Angular frequency for spin motion [in radians/Earth days]

	float obliquityInRad{ 0.0f };			// Obliquity converted [in radians]
	float orbInclinationInRad{ 0.0f };		// Orbital Inclination converted [in radians]

	float distCosOrbInclination{ 0.0f };
	float distSinOrbInclination{ 0.0f };

	float textHeight{ 0.0f };
	float textScale{ 0.0f };
};

// @todo - Sphere is a Mesh, Orbit is a SceneEntity. Create another class as a clear combination of CelestialBody (that would only containing a Sphere) and Orbit (already containing a Circle)? 
// Represent a planet, dwarf planet, moon etc..., its orbit and its name on a billboard
class CelestialBody : public SceneEntity
{
public:
	CelestialBody(BodyData&& inBodyData);
	CelestialBody(CelestialBody&& inCelestialBody);
	~CelestialBody();

	BodyData bodyData;
	std::unique_ptr<BodyRings> bodyRings;
	Orbit orbit;
	Billboard billboard;

	void ComputeCartesianPosition(const float elapsedTime);
	const glm::vec3& GetPosition() const { return position; }

	const PreComputations& GetPreComputations() const { return preComputations; }

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	Sphere sphere;
	PreComputations preComputations;
	std::unique_ptr<LightSource> lightSource;

	// Store body position in Cartesian coordinates, computed from Spherical ones
	glm::vec3 position{ 0.0f };

	// Angle travelled by the planet (resp. moon) around the sun (resp. planet) since the simulation started [in radians]
	float travelledAngle{ 0.0f };

	static Material InitialiseParent(const std::filesystem::path& inTexturePath);
	PreComputations LoadPreComputations();

	void ComputeModelMatrixVUniform(const float elapsedTime = 1.0f) override;
};



#endif // CELESTIAL_BODY_H