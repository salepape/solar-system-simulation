#ifndef BODY_RINGS_H
#define BODY_RINGS_H

#include <filesystem>
#include <string>

#include "SceneEntity.h"
#include "Material.h"
#include "Model.h"

class Renderer;



struct RingsData
{
	std::filesystem::path modelPath;
	std::string bodyName;
	float radius{ 0.0f };
	float opacity{ 0.5f };
};

class BodyRings : public SceneEntity
{
public:
	BodyRings(RingsData&& inRingsData);

	RingsData ringsData;

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	Model model;

	std::string bodyName;

	static Material InitialiseParent(const float inRingsOpacity);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override;
};



#endif // BODY_RINGS_H