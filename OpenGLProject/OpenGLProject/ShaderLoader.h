#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <vector>

#include "Shader.h"
#include "UniformBuffer.h"



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



#endif // SHADER_LOADER_H