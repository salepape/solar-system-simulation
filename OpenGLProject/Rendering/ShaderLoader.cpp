#include "ShaderLoader.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>

#include "Shader.h"
#include "Utils/Helpers.h"

std::vector<Shader> ShaderLibrary::shaders;

std::unordered_map<GLSLUniform::Enum, std::vector<ShaderLookUpID::Enum>> ShaderLibrary::uniformGroups
{
	{
		GLSLUniform::PROJECTION_VIEW,
		{
			ShaderLookUpID::Enum::DEFAULT,
			ShaderLookUpID::Enum::STAR,
			ShaderLookUpID::Enum::BILLBOARD,
			ShaderLookUpID::Enum::BELT,
			ShaderLookUpID::Enum::BACKGROUND,
		}
	},
	{
		GLSLUniform::LINE_OF_SIGHT,
		{
			ShaderLookUpID::Enum::DEFAULT,
			ShaderLookUpID::Enum::BELT,
		}
	}
};



void ShaderLibrary::BuildShaders()
{
	shaders.reserve(ShaderLookUpID::Num);

	const std::string currentProjectPath(FileHelper::GetProjectAbsolutePath() + '/');

	shaders.emplace_back(ShaderLookUpID::Enum::DEFAULT, currentProjectPath + "Rendering/GLSL/DefaultShader.vs", currentProjectPath + "Rendering/GLSL/DefaultShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::STAR, currentProjectPath + "Rendering/GLSL/DefaultShader.vs", currentProjectPath + "Rendering/GLSL/SunShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::BILLBOARD, currentProjectPath + "Rendering/GLSL/BillboardShader.vs", currentProjectPath + "Rendering/GLSL/BillboardShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::BELT, currentProjectPath + "Rendering/GLSL/InstancedModelShader.vs", currentProjectPath + "Rendering/GLSL/DefaultShader.fs");
	shaders.emplace_back(ShaderLookUpID::Enum::BACKGROUND, currentProjectPath + "Rendering/GLSL/SkyboxShader.vs", currentProjectPath + "Rendering/GLSL/SkyboxShader.fs");

	if (shaders.size() != ShaderLookUpID::Num)
	{
		std::cout << "ERROR::SHADER_LOADER - Shader vector count is different than the reserved one: preemptive count is " << ShaderLookUpID::Num << " while actual count is " << static_cast<int>(shaders.size()) << std::endl;
		assert(false);
	}
}

Shader& ShaderLibrary::GetShader(const ShaderLookUpID::Enum inShaderLookUpID)
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

std::vector<ShaderLookUpID::Enum>& ShaderLibrary::GetShaderGroup(const GLSLUniform::Enum inGLSLUniform)
{
	if (uniformGroups.find(inGLSLUniform) == uniformGroups.end())
	{
		std::cout << "ERROR::SHADER_LOADER - Uniform " << GLSLUniform::All[inGLSLUniform] << " has not been found!" << std::endl;
		assert(false);
	}

	return uniformGroups[inGLSLUniform];
}
