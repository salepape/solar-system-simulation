#ifndef MILKY_WAY_H
#define MILKY_WAY_H

#include <string>

#include "Material.h"
#include "SceneEntity.h"
#include "Skybox.h"

class Renderer;



struct MilkyWay : public SceneEntity
{
	Skybox skybox;



	MilkyWay(const std::string& texturePath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override {};

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	static Material InitialiseParent(const std::string& inTexturePath);
};



#endif // MILKY_WAY_H