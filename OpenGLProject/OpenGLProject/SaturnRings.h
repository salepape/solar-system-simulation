#ifndef SATURN_RINGS_H
#define SATURN_RINGS_H

#include <string>

#include "PointLight.h"
#include "SceneEntity.h"
#include "Material.h"
#include "Model.h"

class Renderer;



struct SaturnRings : public SceneEntity
{
	Model model;



	SaturnRings(const std::string& modelPath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override;

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	static Material InitialiseParent(const std::string& inTexturePath);
};



#endif // SATURN_RINGS_H

