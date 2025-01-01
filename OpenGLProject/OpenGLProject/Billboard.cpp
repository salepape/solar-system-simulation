#include "Billboard.h"

#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "CelestialBody.h"
#include "Renderer.h"
#include "TextRenderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Utils.h"



Billboard::Billboard(BodyData&& inBodyData) : SceneEntity(InitialiseParent())
{
	bodyName = ResourceLoader::GetNameFromTexturePath(inBodyData.texturePath);
	name = bodyName + "Billboard";

	float textHeightFactor = 1.5f;
	float textScaleFactor = 0.01f;
	// If the current celestial body is a satellite (i.e. has a parent)
	if (inBodyData.parentID != -1)
	{
		textHeightFactor = 3.5f;
		textScaleFactor = 0.03f;
	}

	textHeight = inBodyData.radius * textHeightFactor;
	textScale = inBodyData.radius * textScaleFactor;
}

Material Billboard::InitialiseParent()
{
	// Add textures after construction finishes via the text renderer passed in argument
	return Material(ResourceLoader::GetShader("TextGlyph"), { /* texturesLoadedFromTheTextRenderer */ });
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

	textRenderer.Render(bodyName, 0.0f, textHeight, textScale);

	shader.Disable();
}