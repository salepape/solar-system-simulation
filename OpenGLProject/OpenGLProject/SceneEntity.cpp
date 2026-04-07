#include "SceneEntity.h"

#include <utility>

#include "Shader.h"

uint32_t SceneEntity::entityIDCounter = 0;



SceneEntity::SceneEntity(std::string inName, BlinnPhongMaterial inMaterial) :
	name(inName), material(std::move(inMaterial))
{
	ID = entityIDCounter++;

	SetVUniforms();
}

void SceneEntity::SetVUniforms() const
{
	Shader& shader = material.GetShader();
	shader.Enable();

	const std::string modelVU("vu_Model");
	if (shader.IsUniformRequired(modelVU.c_str()))
	{
		shader.setUniformMat4(modelVU, glm::mat4(0.0f));
	}

	shader.Disable();
}

void SceneEntity::SetModelMatrixVUniform()
{
	Shader& shader = material.GetShader();
	shader.setUniformMat4("vu_Model", modelMatrix);
}