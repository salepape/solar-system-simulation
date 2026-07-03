#ifndef BODY_RINGS_H
#define BODY_RINGS_H

#include <filesystem>
#include <functional>
#include <optional>
#include <string>

#include "Models/Model.h"
#include "Scene/SceneEntity.h"
#include "Scene/Transform.h"



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
	// IRenderable implementation

private:
	RingsData ringsData;

	// Model used for the Celestial Body "Ring" (contains the Mesh + the baked-in Material definition, as opposed to traditional SceneEntities)
	Model model;

	Transform transform;
	// ITransformable implementation
	const Transform& GetTransform() const override { return transform; }
	void ComputeTransformVUniform(const float deltaTime, const Camera& camera, std::optional<std::reference_wrapper<const ITransformable>> parentTransformable = std::nullopt) override;
	// ITransformable implementation

	std::string bodyParent;
};



#endif // BODY_RINGS_H
