#ifndef MILKY_WAY_H
#define MILKY_WAY_H

#include "SceneEntity.h"
#include "Skybox.h"

class Renderer;



struct MilkyWay : public SceneEntity
{
	Skybox skybox;



	MilkyWay(const std::string& texturePath);

	void ComputeModelMatrixUniform(const float elapsedTime = 0.0f) override {};

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;
};

#endif // MILKY_WAY_H
