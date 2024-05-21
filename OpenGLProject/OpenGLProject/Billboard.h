#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <glm/vec3.hpp>

#include "SceneEntity.h"

struct CelestialBody;
class Renderer;
class TextRenderer;



struct Billboard : public SceneEntity
{
	Billboard(const std::string& legend);

	void ComputeModelMatrixUniform(const float elapsedTime = 0.0f) override {};
	void ComputeModelMatrixUniform(const glm::vec3& forward, const glm::vec3& right);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override {};
	void Render(TextRenderer& textRenderer, const glm::vec3& forward, const glm::vec3& right);

private:
	CelestialBody& body;
};



#endif // BILLBOARD_H