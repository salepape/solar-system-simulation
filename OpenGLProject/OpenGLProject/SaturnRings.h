#ifndef SATURN_RINGS_H
#define SATURN_RINGS_H

#include <string>

#include "PointLight.h"
#include "SceneEntity.h"
#include "Material.h"
#include "Model.h"

class Renderer;



class SaturnRings : public SceneEntity
{
public:
	SaturnRings(const std::string& modelPath);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	Model model;
	PointLight pointLight;

	static Material InitialiseParent(const std::string& inTexturePath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override;
};



#endif // SATURN_RINGS_H