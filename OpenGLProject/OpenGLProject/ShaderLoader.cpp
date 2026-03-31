#include "ShaderLoader.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>

#include "Constants.h"



std::vector<Shader> ShaderLoader::shaders;
std::vector<UniformBuffer> ShaderLoader::ubos;

void ShaderLoader::BuildShaders()
{
	constexpr int numOfShaders = 8;
	shaders.reserve(numOfShaders);

	shaders.emplace_back("CelestialBody", "DefaultShader.vs", "DefaultShader.fs");
	shaders.emplace_back("Sun", "DefaultShader.vs", "SunShader.fs");
	shaders.emplace_back("Billboard", "BillboardShader.vs", "BillboardShader.fs");
	shaders.emplace_back("Belt", "InstancedModelShader.vs", "DefaultShader.fs");
	shaders.emplace_back("MilkyWay", "SkyboxShader.vs", "SkyboxShader.fs");
	shaders.emplace_back("Orbit", "DefaultShader.vs", "DefaultShader.fs");
	shaders.emplace_back("VisibleBodyRings", "DefaultShader.vs", "DefaultShader.fs");
	shaders.emplace_back("InfraredBodyRings", "DefaultShader.vs", "DefaultShader.fs");

	if (shaders.size() != numOfShaders)
	{
		std::cout << "ERROR::SHADER_LOADER - Shader vector count is different than the reserved one: preemptive count is " << numOfShaders << " while actual count is " << static_cast<int>(shaders.size()) << std::endl;
		assert(false);
	}

	constexpr int numOfUBOs = 5;
	ubos.reserve(numOfUBOs);

	ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("Sun").GetRendererID(), GetShader("Belt").GetRendererID(), GetShader("VisibleBodyRings").GetRendererID(), GetShader("InfraredBodyRings").GetRendererID(), GetShader("Orbit").GetRendererID(), GetShader("Billboard").GetRendererID(), GetShader("MilkyWay").GetRendererID() },
		"ubo_ProjectionView", GLSLConstants::mat4v4SizeInBytes);
	ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("Sun").GetRendererID(), GetShader("Belt").GetRendererID(), GetShader("VisibleBodyRings").GetRendererID(), GetShader("InfraredBodyRings").GetRendererID(), GetShader("Orbit").GetRendererID() },
		"ubo_CameraPosition", GLSLConstants::vec4SizeInBytes);

	const std::vector<uint32_t> bodyShaderIDs{ GetShader("CelestialBody").GetRendererID(), GetShader("Belt").GetRendererID(), GetShader("VisibleBodyRings").GetRendererID(), GetShader("InfraredBodyRings").GetRendererID(), GetShader("Orbit").GetRendererID() };
	ubos.emplace_back(bodyShaderIDs, "ubo_DirectionalLight", 4 * GLSLConstants::vec4SizeInBytes + GLSLConstants::scalarSizeInBytes);
	ubos.emplace_back(bodyShaderIDs, "ubo_PointLight", 4 * GLSLConstants::vec4SizeInBytes + 4 * GLSLConstants::scalarSizeInBytes);
	ubos.emplace_back(bodyShaderIDs, "ubo_SpotLight", 5 * GLSLConstants::vec4SizeInBytes + 7 * GLSLConstants::scalarSizeInBytes);

	if (ubos.size() != numOfUBOs)
	{
		std::cout << "ERROR::SHADER_LOADER - UBO vector count is different than the reserved one: preemptive count is " << numOfUBOs << " while actual count is " << static_cast<int>(ubos.size()) << std::endl;
		assert(false);
	}
}

Shader& ShaderLoader::GetShader(const std::string& inShaderLookUpID)
{
	const auto& shaderIt = std::find_if(shaders.begin(), shaders.end(), [&inShaderLookUpID](const Shader& inShader)
	{
		return inShader.GetShaderLookUpID() == inShaderLookUpID;
	});

	if (shaderIt == shaders.end())
	{
		std::cout << "ERROR::SHADER_LOADER - Shader " << inShaderLookUpID << " does not exist!" << std::endl;
		assert(false);
	}

	return *shaderIt;
}

UniformBuffer& ShaderLoader::GetUBO(const std::string& inUniformName)
{
	const auto& uboIt = std::find_if(ubos.begin(), ubos.end(), [&inUniformName](const UniformBuffer& inUBO)
	{
		return inUBO.GetUniformName() == inUniformName;
	});

	if (uboIt == ubos.end())
	{
		std::cout << "ERROR::SHADER_LOADER - Uniform " << inUniformName << " does not exist!" << std::endl;
		assert(false);
	}

	return *uboIt;
}