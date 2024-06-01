#include "MilkyWay.h"

#include "Material.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Shader.h"



MilkyWay::MilkyWay(const std::string& texturePath) : SceneEntity(Material(ResourceLoader::GetShader("Skybox"), { 0.0f, 0.0f, 0.0f }, 0.0f)), skybox({ texturePath })
{
	name = "MilkyWay";
}

void MilkyWay::Render(const Renderer& renderer, const float /*elapsedTime*/)
{
	Shader& shader = material.GetShader();

	shader.Enable();
	material.SetDiffuseSamplerFUniform();
	skybox.Render(renderer, material.GetDiffuseTextureUnit());
	shader.Disable();
}