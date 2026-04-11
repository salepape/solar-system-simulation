#ifndef ORBIT_H
#define ORBIT_H

#include <cstdint>
#include <filesystem>
#include <glm/vec3.hpp>
#include <string>

#include "BlinnPhongMaterial.h"
#include "Circle.h"
#include "SceneEntity.h"

struct BodyData;
class Renderer;



class Orbit : public SceneEntity
{
public:
	Orbit(const BodyData& inBodyData);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override {};
	void Render(const Renderer& renderer, const glm::vec3& parentPosition, const float elapsedTime = 0.0f);

private:
	// @todo - Implement ellipse for a more realistic simulation (circular orbits with eccentricities close to 0 for now)
	CircleMeshComponent circle;
	BlinnPhongMaterial material;

	std::string bodyName;
	bool isMoon{ false };

	// Orbital Inclination converted [in radians]
	float orbInclinationInRad{ 0.0f };

	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& inTexturePath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override {};
	void ComputeModelMatrixVUniform(const glm::vec3& bodyPosition, const float elapsedTime = 0.0f);
};



#endif // ORBIT_H