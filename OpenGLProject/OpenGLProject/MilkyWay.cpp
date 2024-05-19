#include "MilkyWay.h"

#include "Material.h"
#include "ResourceLoader.h"
#include "Shader.h"



MilkyWay::MilkyWay(const std::string& texturePath) : SceneEntity(Material(ResourceLoader::GetShader("Skybox"), { 0.0f, 0.0f, 0.0f }, 0.0f)), skybox({ texturePath })
{
	name = "MilkyWay";
}

void MilkyWay::Render(const Renderer& renderer, uint32_t& textureUnit, const glm::mat4& /*modelMatrix*/)
{
	Shader& shader = GetMaterial().GetShader();

	shader.Enable();
	GetMaterial().SetDiffuseSamplerVUniform(textureUnit);
	skybox.Render(renderer, textureUnit++);
	shader.Disable();
}