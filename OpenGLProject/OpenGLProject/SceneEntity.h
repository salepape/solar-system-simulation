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

	int32_t GetID() const { return ID; }
	const std::string& GetName() const { return name; }

	Material& GetMaterial() { return material; }
	const glm::mat4& GetModelMatrix() const { return modelMatrix; }

	virtual void Render(const Renderer& renderer, const float elapsedTime = 0.0f) = 0;

protected:
	int32_t ID{ -1 };
	std::string name;

	Material material;
	glm::mat4 modelMatrix{ 1.0f };

	virtual void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) = 0;
	void SetModelMatrixVUniform(const glm::mat4& inModelMatrix);

private:
	static uint32_t entityIDCounter;
};



#endif // SCENE_ENTITY_H