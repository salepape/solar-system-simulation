#ifndef ORBIT_H
#define ORBIT_H

#include <filesystem>
#include <memory>
#include <string>

#include "Circle.h"
#include "Material.h"
#include "SceneEntity.h"

struct BodyData;
class Renderer;



class Orbit : public SceneEntity
{
public:
	Orbit(BodyData&& inBodyData);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	// @todo - Implement ellipse for a more realistic simulation (circular orbits with eccentricities close to 0 for now)
	Circle circle;

	std::string bodyName;
	int32_t parentBodyID{ -1 };

	// Orbital Inclination converted [in radians]
	float orbInclinationInRad{ 0.0f };

	static Material InitialiseParent(const std::filesystem::path& inTexturePath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override;
};



#endif // ORBIT_H