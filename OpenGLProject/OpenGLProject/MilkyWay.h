#ifndef MILKY_WAY_H
#define MILKY_WAY_H

#include <filesystem>

#include "BlinnPhongMaterial.h"
#include "Meshes/SkyboxMeshComponent.h"
#include "SceneEntity.h"

class Renderer;



class MilkyWay : public SceneEntity
{
public:
	MilkyWay(const std::filesystem::path& inTexturePath);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	SkyboxMeshComponent skybox;
	BlinnPhongMaterial material;

	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& inTexturePath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override {};
};



#endif // MILKY_WAY_H