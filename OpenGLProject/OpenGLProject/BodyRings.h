#ifndef BODY_RINGS_H
#define BODY_RINGS_H

#include <string>

#include "PointLight.h"
#include "SceneEntity.h"
#include "Material.h"
#include "Model.h"

class Renderer;



class BodyRings : public SceneEntity
{
public:
	BodyRings(const std::string& modelPath, const std::string& inParentBodyName);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	std::string parentBodyName;

	Model model;
	PointLight pointLight;

	static Material InitialiseParent(const std::string& inTexturePath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override;
};



#endif // BODY_RINGS_H