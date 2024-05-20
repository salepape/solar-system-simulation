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

	void Render(const Renderer& renderer, const glm::mat4& modelMatrix) override;
};



#endif // SATURN_RINGS_H

