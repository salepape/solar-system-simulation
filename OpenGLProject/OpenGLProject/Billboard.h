#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <glm/mat4x4.hpp>

#include "SceneEntity.h"

class Renderer;
class TextRenderer;



struct Billboard : public SceneEntity
{
	Billboard(const std::string& legend);

	void Render(const Renderer& renderer, uint32_t& textureUnit, const glm::mat4& modelMatrix) override {};
	void RenderInstances(const Renderer& renderer, uint32_t& textureUnit, const uint32_t instanceCount) override {};

	void Render(TextRenderer& textRenderer, uint32_t& textureUnit, const glm::mat4& modelMatrix);
};



#endif // BILLBOARD_H