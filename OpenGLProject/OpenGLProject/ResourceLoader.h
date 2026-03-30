#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include <string>
#include <vector>

#include "Shader.h"
#include "UniformBuffer.h"



// @todo - To be renamed ShaderLoader
class ResourceLoader
{
public:
	// Build/compile shaders and their corresponding programs
	static void LoadShaders();

	static Shader& GetShader(const std::string& inShaderName);
	static UniformBuffer& GetUBO(const std::string& inUniformName);

private:
	static std::vector<Shader> shaders;
	static std::vector<UniformBuffer> ubos;
};



#endif // RESOURCELOADER_H