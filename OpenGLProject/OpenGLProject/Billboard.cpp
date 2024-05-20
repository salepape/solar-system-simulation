#include "Billboard.h"

#include "Material.h"
#include "Renderer.h"
#include "TextRenderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Utils.h"



Billboard::Billboard(const std::string& legend) : SceneEntity(Material(ResourceLoader::GetShader("Text"), { 0.0f, 0.0f, 0.0f }, 0.0f))
{
	name = legend;
}

void Billboard::Render(TextRenderer& textRenderer, uint32_t& textureUnit, const glm::mat4& modelMatrix)
{
	Shader& shader = material.GetShader();
	const auto& body = ResourceLoader::GetBody(name);

	shader.Enable();
	material.SetDiffuseSamplerVUniform(textureUnit);
	material.SetDiffuseColourVUniform(Utils::whiteColour);
	SetModelMatrixUniform(modelMatrix);
	textRenderer.Render(name, 0.0f, body.preComputations.textHeight, body.preComputations.textScale, textureUnit++);
	shader.Disable();
}