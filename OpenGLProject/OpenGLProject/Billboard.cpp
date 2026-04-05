#include "Billboard.h"

#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "CelestialBody.h"
#include "Constants.h"
#include "Renderer.h"
#include "Shader.h"
#include "ShaderLoader.h"
#include "TextRenderer.h"

namespace
{
	// Only a unique Texture2D Unit needed for all ASCII Glyphs from TextRenderer called by this class
	constexpr uint32_t textureUnit{ 0 };
}



Billboard::Billboard(const BodyData& inBodyData) : SceneEntity(inBodyData.name + "Billboard", InitialiseParent()),
legend(inBodyData.name)
{
	float textHeightFactor = 1.5f;
	float textScaleFactor = 0.01f;
	// If the current celestial body is a satellite (i.e. has a parent)
	if (inBodyData.parentName.length() != 0)
	{
		textHeightFactor = 3.5f;
		textScaleFactor = 0.03f;
	}

	textHeight = inBodyData.radius * textHeightFactor;
	textScale = inBodyData.radius * textScaleFactor;
}

Material Billboard::InitialiseParent()
{
	// All Textures2D used by the Billboard are Glyph Textures2D managed in the Text Renderer directly, so not linked in this Material
	return Material(ShaderLookUpID::Enum::BILLBOARD, { /* texturesLoadedFromTheTextRenderer */ }, { GLMConstants::whiteColour });
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

void Billboard::Render(const Renderer& renderer, TextRenderer& textRenderer, const glm::vec3& bodyPosition, const glm::vec3& cameraForward, const glm::vec3& cameraRight)
{
	ComputeModelMatrixVUniform(bodyPosition, cameraForward, cameraRight);

	Shader& shader = material.GetShader();
	shader.Enable();

	SetModelMatrixVUniform();

	textRenderer.Render(renderer, textureUnit, legend, 0.0f, textHeight, textScale);

	shader.Disable();
}