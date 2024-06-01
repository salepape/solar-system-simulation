#include "SceneEntity.h"

#include <memory>
#include <utility>

#include "Shader.h"

uint32_t SceneEntity::entityIDCounter = 0;



SceneEntity::SceneEntity(Material inMaterial) :
	material(std::move(inMaterial))
{
	ID = entityIDCounter++;
}

void SceneEntity::SetModelMatrixVUniform(const glm::mat4& inModelMatrix)
{
	modelMatrix = inModelMatrix;

	material.GetShader().setUniformMat4("vu_Model", inModelMatrix);
}