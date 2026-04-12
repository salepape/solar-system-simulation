#ifndef BODY_RINGS_H
#define BODY_RINGS_H

#include <filesystem>
#include <glm/mat4x4.hpp>
#include <string>

#include "OpenGLProject/Models/Model.h"
#include "SceneEntity.h"

class Renderer;



struct RingsData
{
	std::filesystem::path modelPath;
	std::string bodyName;
	float radius{ 0.0f };
	float opacity{ 0.5f };
};

class BodyRingsEntity : public SceneEntity
{
public:
	BodyRingsEntity(RingsData&& inRingsData);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override {};
	void Render(const Renderer& renderer, const glm::mat4& modelMatrix, const float elapsedTime = 0.0f);

private:
	RingsData ringsData;

	// Model used for the Celestial Body "Ring" (contains the Mesh + the Material definition, as opposed to traditional SceneEntities)
	Model model;

	std::string bodyName;

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override {};
	void ComputeModelMatrixVUniform(const glm::mat4& modelMatrix, const float elapsedTime = 0.0f);
};



#endif // BODY_RINGS_H