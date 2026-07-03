#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include <glm/vec3.hpp>

#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "Components/Meshes/SphereMeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "SceneEntity.h"

class Camera;
class LightSourceComponent;



struct BodyData
{
	std::filesystem::path texturePath;		// DDS texture path

	std::string name;						// Name of the celestial body
	std::string type;						// Body type (e.g. a star, a planet, etc.)

	float radius{ 0.0f };					// Planet or moon (only those > pluto radius in length) radius divided by Main Planet's radius [in kms]
	float distanceToParent{ 0.0f };			// Distance between planet (resp. moon) and star (resp. the planet around which they gravitate) divided by star-main planet distance (= 1AU) [in kms]
	float obliquity{ 0.0f };				// Or axial tilt: angle between planet/moon axis rotation and the normal of its orbital plane [in degrees]
	float orbitalPeriod{ 0.0f };			// Time (sideral) the planet (resp. moon) takes to do one revolution around the star (resp. its planet) [in Main Planet days]
	float spinPeriod{ 0.0f };				// Time (sideral) the planet takes to do a rotation on itself [in Main Planet days]	
	float orbitalInclination{ 0.0f };		// Or "orbital tilt": angle between planet (resp. moon) orbit and the ecliptic [in degrees]
};

// Represent a spherical mesh body, e.g. a planet, a dwarf planet or a moon
class CelestialBodyEntity : public SceneEntity, public ITransformable, public IRenderable
{
public:
	// Default constructor (not needed)
	CelestialBodyEntity() = delete;

	// User-defined constructor (to be used when building a CelestialBody in-place from initialisation-list)
	CelestialBodyEntity(const BodyData& inBodyData);

	// Copy constructor (not needed - SCENE ENTITY GETTER RETURN NON-OWNING RAW PTR, HENCE NOT NEEDED)
	CelestialBodyEntity(const CelestialBodyEntity& inCelestialBody) = delete;
	CelestialBodyEntity& operator = (const CelestialBodyEntity& inCelestialBody) = delete;

	// Move constructor (not needed - SCENE ENTITY GETTER RETURN NON-OWNING RAW PTR, HENCE NOT NEEDED)
	CelestialBodyEntity(CelestialBodyEntity&& inCelestialBody) = delete;
	CelestialBodyEntity& operator = (CelestialBodyEntity&& inCelestialBody) = delete;

	// Destructor (not virtual needed, as there are no class child classes)
	~CelestialBodyEntity() = default;

	const BodyData& GetBodyData() const { return bodyData; }

	// Compute body position in Cartesian coordinates from Spherical ones
	void ComputeCartesianPosition(const float deltaTime, std::optional<std::reference_wrapper<const SceneEntity>> satelliteParentBody = std::nullopt);
	const glm::vec3& GetPosition() const { return position; }

	// IRenderable implementation
	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& texturePath, const std::string& bodyName) /*override*/;
	void Render() override;
	// IRenderable implementation

private:
	BodyData bodyData;

	SphereMeshComponent sphere;
	BlinnPhongMaterial material;

	std::shared_ptr<LightSourceComponent> lightSource;

	glm::vec3 position{ 0.0f };

	// Angle travelled by the planet (resp. moon) around the star (resp. planet) since the simulation started [in radians]
	float travelledAngle{ 0.0f };

	// Angular frequency for orbital motion [in radians/Main Planet days]
	float orbitAngularFreq{ 0.0f };
	float travelledOrbitAngle{ 0.0f };

	// Angular frequency for spin motion [in radians/Main Planet days]
	float spinAngularFreq{ 0.0f };
	float travelledSpinAngle{ 0.0f };

	// Obliquity converted [in radians]
	float obliquityInRad{ 0.0f };

	float distCosOrbInclination{ 0.0f };
	float distSinOrbInclination{ 0.0f };

	// ITransformable implementation
	void ComputeTransformVUniform(const float deltaTime, const Camera& /*camera*/, std::optional<std::reference_wrapper<const SceneEntity>> parentEntity = std::nullopt) override;
	// ITransformable implementation
};



#endif // CELESTIAL_BODY_H
