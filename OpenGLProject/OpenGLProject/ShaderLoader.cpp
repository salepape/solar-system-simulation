#include "ShaderLoader.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>

#include "Shader.h"



std::vector<Shader> ShaderLoader::shaders;

void ShaderLoader::BuildShaders()
{
	constexpr int numOfShaders = 8;
	shaders.reserve(numOfShaders);

	shaders.emplace_back(ShaderLookUpID::Enum::CELESTIAL_BODY, "DefaultShader.vs", "DefaultShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::SUN, "DefaultShader.vs", "SunShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::BILLBOARD, "BillboardShader.vs", "BillboardShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::BELT, "InstancedModelShader.vs", "DefaultShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::MILKY_WAY, "SkyboxShader.vs", "SkyboxShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::ORBIT, "DefaultShader.vs", "DefaultShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::VISIBLE_BODY_RINGS, "DefaultShader.vs", "DefaultShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::INFRARED_BODY_RINGS, "DefaultShader.vs", "DefaultShader.fs");

	if (shaders.size() != numOfShaders)
	{
		std::cout << "ERROR::SHADER_LOADER - Shader vector count is different than the reserved one: preemptive count is " << numOfShaders << " while actual count is " << static_cast<int>(shaders.size()) << std::endl;
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