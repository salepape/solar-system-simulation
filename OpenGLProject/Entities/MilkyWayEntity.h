#ifndef MILKY_WAY_H
#define MILKY_WAY_H

#include "Components/Meshes/SkyboxMeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "SceneEntity.h"



class MilkyWayEntity : public SceneEntity
{
public:
	MilkyWayEntity();

	void Render();

private:
	SkyboxMeshComponent skybox;
	BlinnPhongMaterial material;

	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& inTexturePath);

	void ComputeModelMatrixVUniform(const float /*deltaTime*/ = 0.0f) override {};
};



#endif // MILKY_WAY_H
