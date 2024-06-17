#ifndef BODY_RINGS_H
#define BODY_RINGS_H

#include <string>

#include "SceneEntity.h"
#include "Material.h"
#include "Model.h"

class Renderer;



class BodyRings : public SceneEntity
{
public:
	BodyRings(const std::string& modelPath, const std::string& inBodyName);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	std::string bodyName;

	Model model;

	static Material InitialiseParent(const std::string& inTexturePath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override;
};



#endif // BODY_RINGS_H