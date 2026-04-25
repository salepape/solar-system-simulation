#ifndef BODY_RINGS_H
#define BODY_RINGS_H

#include <glm/mat4x4.hpp>

#include <filesystem>
#include <string>

#include "Models/Model.h"
#include "SceneEntity.h"



struct RingsData
{
	std::filesystem::path modelPath;
	std::string bodyName;
	float radius{ 0.0f };
};

class BodyRingsEntity : public SceneEntity
{
public:
	BodyRingsEntity(RingsData&& inRingsData);

	void Render(const glm::mat4& modelMatrix);

private:
	RingsData ringsData;

	// Model used for the Celestial Body "Ring" (contains the Mesh + the Material definition, as opposed to traditional SceneEntities)
	Model model;

	std::string bodyName;

	void ComputeModelMatrixVUniform(const float /*elapsedTime*/) override {};
	void ComputeModelMatrixVUniform(const glm::mat4& modelMatrix, const float elapsedTime = 0.0f);
};



#endif // BODY_RINGS_H