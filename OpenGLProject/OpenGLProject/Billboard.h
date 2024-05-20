#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <glm/mat4x4.hpp>

#include "SceneEntity.h"

class Renderer;
class TextRenderer;



struct Billboard : public SceneEntity
{
	Billboard(const std::string& legend);

	void Render(const Renderer& renderer, const glm::mat4& modelMatrix) override {};
	void Render(TextRenderer& textRenderer, const glm::mat4& modelMatrix);
};



#endif // BILLBOARD_H