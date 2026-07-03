#include "MilkyWayEntity.h"

#include <glad/glad.h>

#include <utility>
#include <vector>

#include "Utils/Helpers.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderLoader.h"
#include "Rendering/Texture.h"



MilkyWayEntity::MilkyWayEntity() :
	SceneEntity("MilkyWay"),
	material(InitialiseMaterial(FileHelper::GetSolutionAbsolutePath() + "/Textures/MilkyWay/stars.dds"))
{

}

BlinnPhongMaterial MilkyWayEntity::InitialiseMaterial(const std::filesystem::path& texturePath)
{
	Texture texture(texturePath, GL_TEXTURE_CUBE_MAP, { GL_CLAMP_TO_EDGE }, { GL_LINEAR }, TextureType::Enum::DIFFUSE);
	texture.LoadCubemapDDS();

	return BlinnPhongMaterial(ShaderLookUpID::Enum::BACKGROUND, std::vector<Texture>{ std::move(texture) });
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
