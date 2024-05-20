#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include <glm/mat4x4.hpp>
#include <memory>
#include <string>

#include "Billboard.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Orbit.h"
#include "PointLight.h"
#include "SceneEntity.h"
#include "Sphere.h"

class Renderer;



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

// @todo - Have a better separation body/orbit and naming?
// Represent a planet, dwarf planet, moon etc... and its orbit
struct CelestialBody : public SceneEntity
{
	std::string texturePath;				// DDS texture path
	float radius{ 0.0f };					// Planet or moon (only those > pluto radius in length) radius divided by earth's radius [in kms]
	float distanceToParent{ 0.0f };			// Distance between planet (resp. moon) and sun (resp. the planet around which they gravitate) divided by sun-earth distance [in kms]
	float obliquity{ 0.0f };				// Or axial tilt: angle between planet/moon axis rotation and the normal of its orbital plane [in degrees]
	float orbitalPeriod{ 0.0f };			// Time the planet (resp. moon) takes to do one revolution around the sun (resp. its planet) [in earth days]
	float spinPeriod{ 0.0f };				// Time (sideral) the planet takes to do a rotation on itself [in earth days]	
	float orbitalInclination{ 0.0f };		// Or "orbital tilt": angle between planet (resp. moon) orbit and the ecliptic [in degrees]
	int32_t parentID{ -1 };					// Pointer to the planet mesh around which a moon rotates

	Sphere sphere;							// Sphere mesh
	Orbit orbit;							// Orbit mesh
	PreComputations preComputations;

	std::unique_ptr<Billboard> billboard;

	std::unique_ptr<PointLight> pointLight;
	std::unique_ptr<DirectionalLight> directionalLight;
	
	float travelledAngle{ 0.0f };			// Angle travelled by the planet (resp. moon) around the sun (resp. planet) since the simulation started [in radians]



	CelestialBody(const std::string& inTexturePath, const float inRadius, const float inDistanceToParent, const float inObliquity, const float inOrbitalPeriod, const float inSpinPeriod, const float inOrbitalInclination, const int32_t inParentID = -1);	

	void Render(const Renderer& renderer, const glm::mat4& modelMatrix) override;

private:
	static Material InitialiseParent(const std::string& inTexturePath);
	PreComputations LoadPreComputations();
};



#endif // CELESTIAL_BODY_H