#include "MilkyWayEntity.h"

#include <glad/glad.h>

#include <utility>
#include <vector>

#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderLoader.h"
#include "Rendering/Texture.h"



MilkyWayEntity::MilkyWayEntity(const std::filesystem::path& inTexturePath) : SceneEntity("MilkyWay"),
material(InitialiseMaterial(inTexturePath))
{

}

BlinnPhongMaterial MilkyWayEntity::InitialiseMaterial(const std::filesystem::path& inTexturePath)
{
	Texture texture(inTexturePath, GL_TEXTURE_CUBE_MAP, { GL_CLAMP_TO_EDGE }, { GL_LINEAR }, TextureType::Enum::DIFFUSE);
	texture.LoadCubemapDDS();

	return BlinnPhongMaterial(ShaderLookUpID::Enum::MILKY_WAY, std::vector<Texture>{ std::move(texture) });
}

void MilkyWayEntity::Render()
{
	const Shader& shader = material.GetShader();
	shader.Enable();

	Renderer::SetDepthFctToEqual();

	material.EnableTextures();
	skybox.Render();
	material.DisableTextures();

	Renderer::SetDepthFctToLess();

	shader.Disable();
}