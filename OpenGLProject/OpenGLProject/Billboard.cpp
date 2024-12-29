#include "Billboard.h"

#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "Renderer.h"
#include "TextRenderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Utils.h"



Billboard::Billboard(const std::string& legend) : SceneEntity(InitialiseParent())
{
	name = legend;
}

Material Billboard::InitialiseParent()
{
	// Add textures after construction finishes via the text renderer passed in argument
	return Material(ResourceLoader::GetShader("TextGlyph"), { /* texturesLoadedFromTheTextRenderer */ });
}

void Billboard::SetDataPostConstruction(TextRenderer& textRenderer)
{
	bodyPreComputations = std::make_unique<PreComputations>(ResourceLoader::GetBodySystem(name).GetPreComputations());

	// Texture creation is handled by the Text Renderer for now (glyph rendering issue if textures created in this method)
	textRenderer.LoadASCIICharacters(name);
}

void Billboard::ComputeModelMatrixVUniform(const glm::vec3& bodyPosition, const glm::vec3& forward, const glm::vec3& right)
{
	modelMatrix = glm::mat4(1.0f);

	const glm::vec3& up = cross(forward, right);
	modelMatrix[0] = glm::vec4(right, 0.0f);
	modelMatrix[1] = glm::vec4(up, 0.0f);
	modelMatrix[2] = glm::vec4(forward, 0.0f);
	modelMatrix[3] = glm::vec4(bodyPosition, 1.0f);
}

void Billboard::Render(TextRenderer& textRenderer, const glm::vec3& bodyPosition, const glm::vec3& cameraForward, const glm::vec3& cameraRight)
{
	ComputeModelMatrixVUniform(bodyPosition, cameraForward, cameraRight);

	Shader& shader = material.GetShader();
	shader.Enable();

	SetModelMatrixVUniform(modelMatrix);

	material.SetDiffuseSamplerFUniform();
	material.SetDiffuseColourFUniform(Utils::whiteColour);

	textRenderer.Render(name, 0.0f, bodyPreComputations->textHeight, bodyPreComputations->textScale);

	shader.Disable();
}