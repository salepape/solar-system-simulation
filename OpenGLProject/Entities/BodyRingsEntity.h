#ifndef BODY_RINGS_H
#define BODY_RINGS_H

#include <filesystem>
#include <functional>
#include <optional>
#include <string>

#include "Models/Model.h"
#include "SceneEntity.h"



class Camera;

struct RingsData
{
	std::filesystem::path modelPath;
	std::string bodyParent;
	float radius{ 0.0f };
};

class BodyRingsEntity : public SceneEntity, public ITransformable, public IRenderable
{
public:
	BodyRingsEntity(RingsData&& inRingsData);

	// IRenderable implementation
	void Render() override;
	//BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& texturePath) override { /* already baked in model */ return; }
	// IRenderable implementation

private:
	RingsData ringsData;

	// Model used for the Celestial Body "Ring" (contains the Mesh + the Material definition, as opposed to traditional SceneEntities)
	Model model;

	std::string bodyParent;

	// ITransformable implementation
	void ComputeModelMatrixVUniform(const float deltaTime, const Camera& camera, std::optional<std::reference_wrapper<const SceneEntity>> parentEntity = std::nullopt) override;
	// ITransformable implementation
};



#endif // BODY_RINGS_H
