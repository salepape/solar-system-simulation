#include "SceneEntity.h"

#include <utility>

#include "Shader.h"

uint32_t SceneEntity::entityIDCounter = 0;



SceneEntity::SceneEntity(std::string inName, Material inMaterial) :
	name(inName), material(std::move(inMaterial))
{
	ID = entityIDCounter++;
}

void SceneEntity::SetModelMatrixVUniform(const glm::mat4& inModelMatrix)
{
	modelMatrix = inModelMatrix;

	Shader& shader = material.GetShader();
	shader.setUniformMat4("vu_Model", inModelMatrix);
}