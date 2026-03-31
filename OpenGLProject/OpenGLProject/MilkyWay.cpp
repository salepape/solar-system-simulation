#include "MilkyWay.h"

#include <glad.h>
#include <utility>

#include "Renderer.h"
#include "Shader.h"
#include "ShaderLoader.h"
#include "Texture.h"



MilkyWay::MilkyWay(const std::filesystem::path& inTexturePath) : SceneEntity("MilkyWay", InitialiseParent(inTexturePath))
{

}

Material MilkyWay::InitialiseParent(const std::filesystem::path& inTexturePath)
{
	Texture texture(inTexturePath, GL_TEXTURE_CUBE_MAP, { GL_CLAMP_TO_EDGE }, { GL_LINEAR }, TextureType::Enum::DIFFUSE);
	texture.LoadCubemapDDS();

	return Material(ShaderLookUpID::Enum::MILKY_WAY, { std::move(texture) });
}

void MilkyWay::Render(const Renderer& renderer, const float /*elapsedTime*/)
{
	Shader& shader = material.GetShader();
	shader.Enable();

	renderer.SetDepthFctToEqual();

	material.EnableTextures();
	skybox.Render(renderer);
	material.DisableTextures();

	renderer.SetDepthFctToLess();

	shader.Disable();
}