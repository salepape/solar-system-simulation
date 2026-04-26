#ifndef ORBIT_H
#define ORBIT_H

#include <glm/vec3.hpp>

#include <cstdint>
#include <filesystem>
#include <string>

#include "Components/Meshes/CircleMeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "SceneEntity.h"

struct BodyData;



class OrbitEntity : public SceneEntity
{
public:
	OrbitEntity(const BodyData& inBodyData);

	void Render(const glm::vec3& parentPosition);

private:
	// @todo - Implement ellipse for a more realistic simulation (circular orbits with eccentricities close to 0 for now)
	CircleMeshComponent circle;
	BlinnPhongMaterial material;

	std::string bodyName;
	bool isMoon{ false };

	// Orbital Inclination converted [in radians]
	float orbInclinationInRad{ 0.0f };

	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& inTexturePath);

	void ComputeModelMatrixVUniform(const float /*elapsedTime*/) override {};
	void ComputeModelMatrixVUniform(const glm::vec3& bodyPosition);
};



#endif // ORBIT_H
