#include "ResourceLoader.h"

#include <algorithm>
#include <cstdint>
#include <iostream>

#include "Constants.h"



std::vector<Shader> ShaderLoader::shaders;
std::vector<UniformBuffer> ShaderLoader::ubos;

void ShaderLoader::BuildShaders()
{
	// @todo - Find a solution to avoid having to update this number when adding a new shader, or a warning at the very least (white screen otherwise!)
	shaders.reserve(8);

	// RendererID will be identical for all shaders if we do not instantiate them line by line before pushing them into the vector
	shaders.emplace_back("CelestialBody", "DefaultShader.vs", "DefaultShader.fs");
	shaders.emplace_back("Sun", "DefaultShader.vs", "SunShader.fs");
	shaders.emplace_back("Billboard", "BillboardShader.vs", "BillboardShader.fs");
	shaders.emplace_back("Belt", "InstancedModelShader.vs", "DefaultShader.fs");
	shaders.emplace_back("MilkyWay", "SkyboxShader.vs", "SkyboxShader.fs");
	shaders.emplace_back("Orbit", "DefaultShader.vs", "DefaultShader.fs");
	shaders.emplace_back("VisibleBodyRings", "DefaultShader.vs", "DefaultShader.fs");
	shaders.emplace_back("InfraredBodyRings", "DefaultShader.vs", "DefaultShader.fs");

	// @todo - Find a solution to avoid having to update this number when adding a new UBO, or a warning at the very least (white screen otherwise!)
	ubos.reserve(5);

	ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("Sun").GetRendererID(), GetShader("Belt").GetRendererID(), GetShader("VisibleBodyRings").GetRendererID(), GetShader("InfraredBodyRings").GetRendererID(), GetShader("Orbit").GetRendererID(), GetShader("Billboard").GetRendererID(), GetShader("MilkyWay").GetRendererID() },
		"ubo_ProjectionView", GLSLConstants::mat4v4SizeInBytes);
	ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("Sun").GetRendererID(), GetShader("Belt").GetRendererID(), GetShader("VisibleBodyRings").GetRendererID(), GetShader("InfraredBodyRings").GetRendererID(), GetShader("Orbit").GetRendererID() },
		"ubo_CameraPosition", GLSLConstants::vec4SizeInBytes);

	const std::vector<uint32_t> bodyShaderIDs{ GetShader("CelestialBody").GetRendererID(), GetShader("Belt").GetRendererID(), GetShader("VisibleBodyRings").GetRendererID(), GetShader("InfraredBodyRings").GetRendererID(), GetShader("Orbit").GetRendererID() };
	ubos.emplace_back(bodyShaderIDs, "ubo_DirectionalLight", 4 * GLSLConstants::vec4SizeInBytes + GLSLConstants::scalarSizeInBytes);
	ubos.emplace_back(bodyShaderIDs, "ubo_PointLight", 4 * GLSLConstants::vec4SizeInBytes + 4 * GLSLConstants::scalarSizeInBytes);
	ubos.emplace_back(bodyShaderIDs, "ubo_SpotLight", 5 * GLSLConstants::vec4SizeInBytes + 7 * GLSLConstants::scalarSizeInBytes);
}

Shader& ShaderLoader::GetShader(const std::string& inShaderName)
{
	const auto& shaderIt = std::find_if(shaders.begin(), shaders.end(), [&inShaderName](const Shader& inShader)
	{
		return inShader.GetEntityName() == inShaderName;
	});

	if (shaderIt == shaders.end())
	{
		std::cout << "ERROR::SHADER_LOADER - Shader " << inShaderName << " does not exist!" << std::endl;
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
	}

	return *uboIt;
}