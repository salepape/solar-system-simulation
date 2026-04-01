#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <vector>

#include "Shader.h"



// Util static class giving a global access point to all Shaders applied on Scene Entity/Objects of the simulation
class ShaderLoader
{
public:
	// Instantiate Shaders for all Scene Entity/Objects of the simulation based on GLSL Vertex/Fragment Shaders, and common Uniform variables
	static void BuildShaders();

	static Shader& GetShader(const ShaderLookUpID::Enum inShaderName);

private:
	static std::vector<Shader> shaders;
};



#endif // SHADER_LOADER_H