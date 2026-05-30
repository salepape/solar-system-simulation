#ifndef MILKY_WAY_H
#define MILKY_WAY_H

#include <filesystem>

#include "Components/Meshes/SkyboxMeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "SceneEntity.h"



class MilkyWayEntity : public SceneEntity, public IRenderable
{
public:
	MilkyWayEntity();

	// IRenderable implementation
	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& texturePath) /*override*/;
	void Render() override;
	// IRenderable implementation

private:
	SkyboxMeshComponent skybox;
	BlinnPhongMaterial material;
};



#endif // MILKY_WAY_H
