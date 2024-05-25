#ifndef SATURN_RINGS_H
#define SATURN_RINGS_H

#include <string>

#include "PointLight.h"
#include "SceneEntity.h"
#include "Model.h"

class Renderer;



struct SaturnRings : public SceneEntity
{
	Model model;

	PointLight pointLight;



	SaturnRings(const std::string& texturePath);

	void ComputeModelMatrixUniform(const float elapsedTime = 0.0f) override;

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;
};



#endif // SATURN_RINGS_H

