#ifndef ORBIT_H
#define ORBIT_H

#include <filesystem>
#include <functional>
#include <optional>
#include <string>

#include "Components/Meshes/CircleMeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "Scene/SceneEntity.h"
#include "Scene/Transform.h"

struct BodyData;
class Camera;



class OrbitEntity : public SceneEntity, public ITransformable, public IRenderable
{
public:
	// Note: we assume the same texture sprite/parameters as the Celestial Body parent ones are used
	OrbitEntity(const BodyData& inBodyData, const float inAlpha = 1.0f);

	// IRenderable implementation
	void Render() override;
	// IRenderable implementation

private:
	// @todo - Implement ellipse for a more realistic simulation (circular orbits with eccentricities close to 0 for now)
	CircleMeshComponent circle;

	BlinnPhongMaterial material;
	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& texturePath, const float inAlpha);

	Transform transform;
	// ITransformable implementation
	const Transform& GetTransform() const override { return transform; }
	void ComputeTransformVUniform(const float deltaTime, const Camera& camera, std::optional<std::reference_wrapper<const ITransformable>> parentTransformable = std::nullopt) override;
	// ITransformable implementation

	std::string bodyName;

	// Orbital Inclination converted [in radians]
	float orbInclinationInRad{ 0.0f };
};



#endif // ORBIT_H
