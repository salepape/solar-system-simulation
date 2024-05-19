#ifndef ORBIT_H
#define ORBIT_H

#include <glm/mat4x4.hpp>

#include "Circle.h"
#include "PointLight.h"
#include "SceneEntity.h"

class Renderer;



struct Orbit : public SceneEntity
{
	// @todo - Implement ellipse for a more realistic simulation (circular orbits with eccentricities close to 0 for now)
	Circle circle;

	PointLight pointLight;



	Orbit(const std::string& texturePath, const float radius);

	void Render(const Renderer& renderer, uint32_t& textureUnit, const glm::mat4& modelMatrix) override;
	void RenderInstances(const Renderer& renderer, uint32_t& textureUnit, const uint32_t instanceCount) override {};
};



#endif // ORBIT_H

