#include "MilkyWay.h"

#include <glad.h>
#include <utility>

#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Texture.h"



MilkyWay::MilkyWay(const std::string& texturePath) : SceneEntity(InitialiseParent(texturePath))
{
	name = "MilkyWay";
}

Material MilkyWay::InitialiseParent(const std::string& inTexturePath)
{
	Texture texture(inTexturePath, GL_TEXTURE_CUBE_MAP, { GL_CLAMP_TO_EDGE }, { GL_LINEAR }, TextureType::DIFFUSE);
	texture.LoadCubemapDDS();

	return Material(ResourceLoader::GetShader("MilkyWay"), { std::move(texture) });
}

void MilkyWay::Render(const Renderer& renderer, const float /*elapsedTime*/)
{
	Shader& shader = material.GetShader();
	shader.Enable();

	material.SetDiffuseSamplerFUniform();

	renderer.SetDepthFctToEqual();

	material.EnableTextures();
	skybox.Render(renderer);
	material.DisableTextures();

	renderer.SetDepthFctToLess();

	shader.Disable();
}