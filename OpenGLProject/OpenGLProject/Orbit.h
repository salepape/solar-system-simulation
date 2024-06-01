#ifndef ORBIT_H
#define ORBIT_H

#include <memory>
#include <string>

#include "Circle.h"
#include "Material.h"
#include "PointLight.h"
#include "SceneEntity.h"

struct PreComputations;
class Renderer;



struct Orbit : public SceneEntity
{
	// @todo - Implement ellipse for a more realistic simulation (circular orbits with eccentricities close to 0 for now)
	Circle circle;

	PointLight pointLight;


	Orbit(const std::string& texturePath, const float radius);
	void SetDataPostConstruction();

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override;

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;
	
	std::string bodyName;
	int32_t bodyID{ -1 };
	int32_t parentBodyID{ -1 };
	std::unique_ptr<PreComputations> bodyPreComputations;

private:
	static Material InitialiseParent(const std::string& inTexturePath);
};



#endif // ORBIT_H

