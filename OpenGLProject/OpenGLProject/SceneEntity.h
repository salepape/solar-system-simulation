#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <glm/mat4x4.hpp>
#include <string>

#include "Material.h"

class Renderer;



// Class that represents a 'Game Object'
class SceneEntity
{
public:
	SceneEntity(Material inMaterial);

	// @todo - Reduce visibility of methods below?

	void ComputeModelMatrixUniform();
	void SetModelMatrixUniform(const glm::mat4& inModelMatrix);

	virtual void Render(const Renderer& renderer, uint32_t& textureUnit, const glm::mat4& modelMatrix) = 0;
	virtual void RenderInstances(const Renderer& renderer, uint32_t& textureUnit, const uint32_t instanceCount) = 0;

	Material& GetMaterial() { return material; }

	std::string name;
	int32_t ID{ -1 };

protected:
	static uint32_t entityIDCounter;

	Material material;
	glm::mat4 modelMatrix;
};



#endif // SCENE_ENTITY_H