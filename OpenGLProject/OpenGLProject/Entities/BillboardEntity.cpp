#include "BillboardEntity.h"

#include <cstdint>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <utility>
#include <vector>

#include "CelestialBodyEntity.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderLoader.h"
#include "Rendering/TextRenderer.h"
#include "Rendering/Texture.h"
#include "Utils/Constants.h"

namespace
{
	// Only a unique Texture2D Unit needed for all ASCII Glyphs from Glyph Loader called by this class
	constexpr uint32_t textureUnit{ 0 };
}



// @todo - Avoid magic numbers and automate it as much as possible from a UI dimensions point of view
BillboardEntity::BillboardEntity(const BodyData& inBodyData) : SceneEntity(inBodyData.name + "Billboard"),
legend(inBodyData.name),
isMoon(inBodyData.parentName.length() != 0),
glyphAdvanceScaleFactor(inBodyData.radius * (isMoon ? 0.03f : 0.01f)),
quads(ComputeQuadParams(0.0f, inBodyData.radius * (isMoon ? 3.5f : 1.5f))),
material(InitialiseMaterial())
{

}

BlinnPhongMaterial BillboardEntity::InitialiseMaterial()
{
	// All Textures2D used by the Billboard are created by the Glyph Loader and globally accessible, so not linked in this Material
	return BlinnPhongMaterial(ShaderLookUpID::Enum::BILLBOARD, std::vector<Texture>{ /* texturesLoadedFromTheGlyphLoader */ }, DiffuseProperties{ GLMConstants::whiteColour });
}

void BillboardEntity::ComputeModelMatrixVUniform(const glm::vec3& bodyPosition, const glm::vec3& forward, const glm::vec3& right)
{
	modelMatrix = glm::mat4(1.0f);

	const glm::vec3& up = cross(forward, right);
	modelMatrix[0] = glm::vec4(right, 0.0f);
	modelMatrix[1] = glm::vec4(up, 0.0f);
	modelMatrix[2] = glm::vec4(forward, 0.0f);
	modelMatrix[3] = glm::vec4(bodyPosition, 1.0f);
}

std::vector<QuadParams> BillboardEntity::ComputeQuadParams(const float billboardXStart, const float billboardYStart)
{
	std::vector<QuadParams> quadPerGlyph;
	quadPerGlyph.reserve(legend.length());

	// Left-shift billboard X position to half its width to center-align it to the body
	float quadXStart = billboardXStart - 0.5f * ComputeBillboardWidth();
	const float quadYStart = billboardYStart;

	// Instantiate a quad to render each character of the legend
	for (const char& character : legend)
	{
		const GlyphParams& glyphParams = GlyphLoader::GetGlyphParams(static_cast<int8_t>(character));
		if (glyphParams.textures.size() <= 0)
		{
			std::cout << "ERROR::BILLBOARD - No Glyph Texture2D has been generated for character " << character << " - check if it is supported in the ASCII table" << std::endl;
			assert(false);
		}

		// Position of the glyph quad
		const float xPosition = quadXStart + glyphParams.bearing.x * glyphAdvanceScaleFactor;
		const float yPosition = quadYStart - (glyphParams.height - glyphParams.bearing.y) * glyphAdvanceScaleFactor;

		// Size of the glyph quad
		const float width = glyphParams.width * glyphAdvanceScaleFactor;
		const float height = glyphParams.height * glyphAdvanceScaleFactor;

		quadPerGlyph.emplace_back(QuadParams{ xPosition, yPosition, width, height });

		quadXStart += GetGlyphAdvance(glyphParams);
	}

	return quadPerGlyph;
}

float BillboardEntity::ComputeBillboardWidth() const
{
	float totalAdvance = 0.0f;
	for (const char& character : legend)
	{
		totalAdvance += GetGlyphAdvance(GlyphLoader::GetGlyphParams(static_cast<int8_t>(character)));
	}

	return totalAdvance;
}

float BillboardEntity::GetGlyphAdvance(const GlyphParams& glyphParams) const
{
	// Bitshift advance by VERTICES_COUNT to get value in pixels (2^QUAD_VERTICES_COUNT = 64, so we divide 1/64th pixels by 64 to get the amount of pixels)
	return (glyphParams.advance >> QuadMeshComponent::QUAD_VERTICES_COUNT) * glyphAdvanceScaleFactor;
}

void BillboardEntity::Render(const glm::vec3& bodyPosition, const glm::vec3& cameraForward, const glm::vec3& cameraRight)
{
	ComputeModelMatrixVUniform(bodyPosition, cameraForward, cameraRight);

	const Shader& shader = material.GetShader();
	shader.Enable();

	Renderer::SetModelMatrixVUniform(shader, modelMatrix);

	quads.RenderGlyphs(legend, textureUnit);

	shader.Disable();
}