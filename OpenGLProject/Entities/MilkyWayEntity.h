#ifndef MILKY_WAY_H
#define MILKY_WAY_H

#include <filesystem>

#include "Components/Meshes/SkyboxMeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "Scene/SceneEntity.h"



// @todo - Name it differently: other Star-based systems will not have Milky Way as background
class MilkyWayEntity : public SceneEntity, public IRenderable
{
public:
	MilkyWayEntity();

	// IRenderable implementation
	void Render() override;
	// IRenderable implementation

private:
	SkyboxMeshComponent skybox;

	BlinnPhongMaterial material;
	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& texturePath);
};



#endif // MILKY_WAY_H
