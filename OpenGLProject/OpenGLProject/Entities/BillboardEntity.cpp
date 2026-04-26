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
#include "Rendering/GlyphLoader.h"
#include "Rendering/Texture.h"
#include "Utils/Constants.h"

namespace
{
	// Only a unique Texture2D Unit needed for all ASCII Glyphs from Glyph Loader called by this class
	constexpr uint32_t textureUnit{ 0 };

	constexpr float bodyGlyphTextureScaleFactor = 0.01f;
	constexpr float moonGlyphTextureScaleFactor = 0.03f;

	constexpr float bodyBilboardYStartScaleFactor = 1.5f;
	constexpr float moonBilboardYStartScaleFactor = 3.5f;
}



BillboardEntity::BillboardEntity(const BodyData& inBodyData) : SceneEntity(inBodyData.name + "Billboard"),
legend(inBodyData.name),
isMoon(inBodyData.parentName.length() != 0),
glyphTextureScaleFactor(inBodyData.radius * (isMoon ? moonGlyphTextureScaleFactor : bodyGlyphTextureScaleFactor)),
quads(ComputeQuadParams(0.0f, inBodyData.radius * (isMoon ? moonBilboardYStartScaleFactor : bodyBilboardYStartScaleFactor))),
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
		const float xPosition = quadXStart + glyphParams.bearing.x * glyphTextureScaleFactor;
		const float yPosition = quadYStart - (glyphParams.height - glyphParams.bearing.y) * glyphTextureScaleFactor;

		// Size of the glyph quad
		const float width = glyphParams.width * glyphTextureScaleFactor;
		const float height = glyphParams.height * glyphTextureScaleFactor;

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
	// Get value in pixels by a bitshift (2^QUAD_VERTEX_COUNT = 64, so we divide 1/64th pixels by 64 to get the amount of pixels)
	return (glyphParams.advance >> QuadMeshComponent::QUAD_VERTEX_COUNT) * glyphTextureScaleFactor;
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
