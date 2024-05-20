#ifndef MILKY_WAY_H
#define MILKY_WAY_H

#include <glm/mat4x4.hpp>

#include "SceneEntity.h"
#include "Skybox.h"

class Renderer;



struct MilkyWay : public SceneEntity
{
	Skybox skybox;



	MilkyWay(const std::string& texturePath);

	void Render(const Renderer& renderer, const glm::mat4& modelMatrix = glm::mat4(1.0f)) override;
};

#endif // MILKY_WAY_H
