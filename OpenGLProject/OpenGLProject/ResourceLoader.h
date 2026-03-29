#pragma once

#include <string>

class Shader;
class UniformBuffer;



// @todo - Convert it to a pure static util class
namespace ResourceLoader
{
	Shader& GetShader(const std::string& inShaderName);
	UniformBuffer& GetUBO(const std::string& inUniformName);

	// Build/compile shaders and their corresponding programs
	void LoadShaders();
}