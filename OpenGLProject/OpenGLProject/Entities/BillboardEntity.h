#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <glm/vec3.hpp>

#include <cstdint>
#include <string>

#include "Rendering/BlinnPhongMaterial.h"
#include "SceneEntity.h"

struct BodyData;
class Renderer;
class TextRenderer;



class BillboardEntity : public SceneEntity
{
public:
	BillboardEntity(const BodyData& inBodyData);

	void Render(const Renderer& /*renderer*/, const float /*elapsedTime*/) override {};
	void Render(const Renderer& renderer, TextRenderer& textRenderer, const glm::vec3& bodyPosition, const glm::vec3& cameraForward, const glm::vec3& cameraRight);

private:
	// No Mesh instance here: directly managed by the Text Renderer
	BlinnPhongMaterial material;

	std::string legend;

	float textHeight{ 0.0f };
	float textScale{ 0.0f };

	BlinnPhongMaterial InitialiseMaterial();

	void ComputeModelMatrixVUniform(const float /*elapsedTime*/) override {};
	void ComputeModelMatrixVUniform(const glm::vec3& bodyPosition, const glm::vec3& cameraForward, const glm::vec3& cameraRight);
};



#endif // BILLBOARD_H