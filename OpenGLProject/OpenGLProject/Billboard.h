#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <cstdint>
#include <glm/vec3.hpp>
#include <string>

#include "BlinnPhongMaterial.h"
#include "SceneEntity.h"

struct BodyData;
class Renderer;
class TextRenderer;



class Billboard : public SceneEntity
{
public:
	Billboard(const BodyData& inBodyData);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override {};
	void Render(const Renderer& renderer, TextRenderer& textRenderer, const glm::vec3& bodyPosition, const glm::vec3& cameraForward, const glm::vec3& cameraRight);

private:
	std::string legend;

	float textHeight{ 0.0f };
	float textScale{ 0.0f };

	BlinnPhongMaterial InitialiseParent();

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override {};
	void ComputeModelMatrixVUniform(const glm::vec3& bodyPosition, const glm::vec3& cameraForward, const glm::vec3& cameraRight);
};



#endif // BILLBOARD_H