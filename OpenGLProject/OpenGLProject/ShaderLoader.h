#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <string>
#include <vector>

#include "Shader.h"
#include "UniformBuffer.h"



// @todo - To be renamed ShaderLoader
class ShaderLoader
{
public:
	// Build/compile shaders, their corresponding programs, and register all needed Uniform Buffer variables (common across several shaders)
	static void BuildShaders();

	static Shader& GetShader(const std::string& inShaderName);
	static UniformBuffer& GetUBO(const std::string& inUniformName);

private:
	static std::vector<Shader> shaders;
	static std::vector<UniformBuffer> ubos;
};



#endif // SHADERLOADER_H