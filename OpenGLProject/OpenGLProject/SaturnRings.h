#ifndef SATURN_RINGS_H
#define SATURN_RINGS_H

#include <glm/mat4x4.hpp>

#include "PointLight.h"
#include "SceneEntity.h"
#include "Model.h"

class Renderer;



struct SaturnRings : public SceneEntity
{
	Model model;

	PointLight pointLight;



	SaturnRings(const std::string& texturePath);

	void Render(const Renderer& renderer, uint32_t& textureUnit, const glm::mat4& modelMatrix) override;
	void RenderInstances(const Renderer& renderer, uint32_t& textureUnit, const uint32_t instanceCount) override {};
};



#endif // SATURN_RINGS_H

