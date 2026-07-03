#ifndef GALAXY_BACKGROUND_H
#define GALAXY_BACKGROUND_H

#include <filesystem>
#include <string>

#include "Components/Meshes/SkyboxMeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "Scene/SceneEntity.h"



class GalaxyBackgroundEntity : public SceneEntity, public IRenderable
{
public:
	GalaxyBackgroundEntity(const std::filesystem::path& inTexturePath, const std::string& inName);

	// IRenderable implementation
	void Render() override;
	// IRenderable implementation

private:
	SkyboxMeshComponent skybox;

	BlinnPhongMaterial material;
	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& texturePath);
};



#endif // GALAXY_BACKGROUND_H
