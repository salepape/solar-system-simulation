#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <glm/vec3.hpp>
#include <memory>
#include <string>

#include "Material.h"
#include "SceneEntity.h"

struct PreComputations;
class Renderer;
class TextRenderer;



struct Billboard : public SceneEntity
{
	Billboard(const std::string& legend);
	void SetDataPostConstruction(TextRenderer& textRenderer);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override {};
	void ComputeModelMatrixVUniform(const glm::vec3& bodyPosition, const glm::vec3& cameraForward, const glm::vec3& cameraRight);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override {};
	void Render(TextRenderer& textRenderer, const glm::vec3& bodyPosition, const glm::vec3& cameraForward, const glm::vec3& cameraRight);

private:
	std::unique_ptr<PreComputations> bodyPreComputations;

	static Material InitialiseParent();
};



#endif // BILLBOARD_H