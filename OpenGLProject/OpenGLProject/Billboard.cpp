#include "Billboard.h"

#include <glm/geometric.hpp>
#include <glm/vec4.hpp>

#include "Material.h"
#include "Renderer.h"
#include "TextRenderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Utils.h"



Billboard::Billboard(const std::string& legend) : SceneEntity(Material(ResourceLoader::GetShader("Text"), { 0.0f, 0.0f, 0.0f }, 0.0f)), 
body(ResourceLoader::GetBody(legend))
{
	name = legend;
}

void Billboard::ComputeModelMatrixUniform(const glm::vec3& forward, const glm::vec3& right)
{
	const glm::vec3& up = cross(forward, right);
	modelMatrix[0] = glm::vec4(right, 0.0f);
	modelMatrix[1] = glm::vec4(up, 0.0f);
	modelMatrix[2] = glm::vec4(forward, 0.0f);
	modelMatrix[3] = glm::vec4(body.GetPosition(), 1.0f);
}

void Billboard::Render(TextRenderer& textRenderer, const glm::vec3& forward, const glm::vec3& right)
{
	ComputeModelMatrixUniform(forward, right);

	Shader& shader = material.GetShader();

	shader.Enable();
	material.SetDiffuseSamplerFUniform();
	material.SetDiffuseColourFUniform(Utils::whiteColour);
	SetModelMatrixUniform(modelMatrix);
	textRenderer.Render(name, 0.0f, body.preComputations.textHeight, body.preComputations.textScale, material.GetDiffuseTextureUnit());
	shader.Disable();
}