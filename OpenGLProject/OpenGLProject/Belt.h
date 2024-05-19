#ifndef BELT_H
#define BELT_H

#include <glm/mat4x4.hpp>
#include <iostream>
#include <vector>

#include "Model.h"
#include "PointLight.h"
#include "SceneEntity.h"

class Renderer;



// Instance is an asteroid for a belt
struct InstanceParams
{
	std::string texturePath;

	const uint32_t count{ 0 };

	// Range from where a random number will be picked from to determine the model size
	const float sizeRangeLowerBound{ 0.0f };
	const uint32_t sizeRangeSpan{ 0 };
};

// Torus built with instances (not with vertices like a real geometrical shape)
struct TorusParams
{
	// Distance tube center - torus center
	const float majorRadius{ 0.0f };

	// Tube radius
	const float minorRadius{ 0.0f };

	// Factor reducing y-coordinate value for each instance
	const float flatnessFactor{ 0.0f };
};

class Belt : public SceneEntity
{
public:
	Belt(const InstanceParams& inInstanceParams, const TorusParams& inTorusParams);

	void Render(const Renderer& renderer, uint32_t& textureUnit, const glm::mat4& modelMatrix = glm::mat4(1.0f)) override;
	void RenderInstances(const Renderer& renderer, uint32_t& textureUnit, const uint32_t instanceCount) override {};

private:
	InstanceParams instanceParams;
	TorusParams torusParams;
	std::vector<glm::mat4> modelMatrices;

	// Model used for a belt body
	Model model;

	PointLight pointLight;

	void ComputeInstanceModelMatrices();
	void StoreInstanceModelMatrices() const;
};



#endif // BELT_H