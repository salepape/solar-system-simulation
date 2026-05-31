#ifndef ORBIT_H
#define ORBIT_H

#include <filesystem>
#include <functional>
#include <optional>
#include <string>

#include "Components/Meshes/CircleMeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "Scene/SceneEntity.h"

struct BodyData;
class Camera;



class OrbitEntity : public SceneEntity, public ITransformable, public IRenderable
{
public:
	OrbitEntity(const BodyData& inBodyData);

	// IRenderable implementation
	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& texturePath) /*override*/;
	void Render() override;
	// IRenderable implementation

private:
	// @todo - Implement ellipse for a more realistic simulation (circular orbits with eccentricities close to 0 for now)
	CircleMeshComponent circle;
	BlinnPhongMaterial material;

	std::string bodyName;

	// Orbital Inclination converted [in radians]
	float orbInclinationInRad{ 0.0f };

	// ITransformable implementation
	void ComputeModelMatrixVUniform(const float deltaTime, const Camera& camera, std::optional<std::reference_wrapper<const SceneEntity>> parentEntity = std::nullopt) override;
	// ITransformable implementation
};



#endif // ORBIT_H
