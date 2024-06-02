#ifndef MILKY_WAY_H
#define MILKY_WAY_H

#include <string>

#include "Material.h"
#include "SceneEntity.h"
#include "Skybox.h"

class Renderer;



class MilkyWay : public SceneEntity
{
public:
	MilkyWay(const std::string& texturePath);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	Skybox skybox;

	static Material InitialiseParent(const std::string& inTexturePath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override {};
};



#endif // MILKY_WAY_H