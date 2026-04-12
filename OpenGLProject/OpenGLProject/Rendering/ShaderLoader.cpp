#include "ShaderLoader.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>

#include "Shader.h"



std::vector<Shader> ShaderLoader::shaders;

void ShaderLoader::BuildShaders()
{
	shaders.reserve(ShaderLookUpID::Num);

	shaders.emplace_back(ShaderLookUpID::Enum::CELESTIAL_BODY, "Rendering/GLSL/DefaultShader.vs", "Rendering/GLSL/DefaultShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::SUN, "Rendering/GLSL/DefaultShader.vs", "Rendering/GLSL/SunShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::BILLBOARD, "Rendering/GLSL/BillboardShader.vs", "Rendering/GLSL/BillboardShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::BELT, "Rendering/GLSL/InstancedModelShader.vs", "Rendering/GLSL/DefaultShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::MILKY_WAY, "Rendering/GLSL/SkyboxShader.vs", "Rendering/GLSL/SkyboxShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::ORBIT, "Rendering/GLSL/DefaultShader.vs", "Rendering/GLSL/DefaultShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::VISIBLE_BODY_RINGS, "Rendering/GLSL/DefaultShader.vs", "Rendering/GLSL/DefaultShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::INFRARED_BODY_RINGS, "Rendering/GLSL/DefaultShader.vs", "Rendering/GLSL/DefaultShader.fs");

	if (shaders.size() != ShaderLookUpID::Num)
	{
		std::cout << "ERROR::SHADER_LOADER - Shader vector count is different than the reserved one: preemptive count is " << ShaderLookUpID::Num << " while actual count is " << static_cast<int>(shaders.size()) << std::endl;
		assert(false);
	}
}

Shader& ShaderLoader::GetShader(const ShaderLookUpID::Enum inShaderLookUpID)
{
	const auto& shaderIt = std::find_if(shaders.begin(), shaders.end(), [&inShaderLookUpID](const Shader& inShader)
	{
		return inShader.GetShaderLookUpID() == inShaderLookUpID;
	});

	if (shaderIt == shaders.end())
	{
		std::cout << "ERROR::SHADER_LOADER - Shader " << ShaderLookUpID::Get(inShaderLookUpID) << " does not exist!" << std::endl;
		assert(false);
	}

	return *shaderIt;
}