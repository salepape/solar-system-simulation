#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

class Shader;

// @todo - Re-arrange so code is independant from the simulation ("Engine" code)

// To be used to refer to any Shader instead of relying on raw strings (layer of security over the existence of LookUpIDs when instantiating or look-up functions)
namespace ShaderLookUpID
{
	static const size_t Num = 5;

	// Enum elements do not correspond to GLSL Shader names but on which Scene Entity/Object Mesh they are applied to
	enum Enum
	{
		UNDEFINED = 0,
		DEFAULT,
		SUN,
		BILLBOARD,
		MILKY_WAY,
		BELT,
	};

	static const std::array<Enum, Num> All = { DEFAULT, SUN, BILLBOARD, BELT, MILKY_WAY, };

	// [[maybe_unused]] only used here to silence a Clang warning!
	[[maybe_unused]] static Enum Get(const int index) { return All[index]; }
};

// GLSL Uniforms shared across Vertex/Fragment Shaders
namespace GLSLUniform
{
	// All Shaders applied on Scene Entities/Objects affected by Uniform in real-time
	enum Enum
	{
		PROJECTION_VIEW = 0,
		LINE_OF_SIGHT
	};

	static const std::array<Enum, 2> All = { PROJECTION_VIEW, LINE_OF_SIGHT, };
};

// Util static class giving a global access point to all Shaders applied on Scene Entity/Objects of the simulation
class ShaderLoader
{
public:
	// Instantiate Shaders for all Scene Entity/Objects of the simulation based on GLSL Vertex/Fragment Shaders, and common Uniform variables
	static void BuildShaders();

	static Shader& GetShader(const ShaderLookUpID::Enum inShaderName);
	static std::vector<ShaderLookUpID::Enum>& GetShaderGroup(const GLSLUniform::Enum inGLSLUniform);

private:
	static std::vector<Shader> shaders;
	static std::unordered_map<GLSLUniform::Enum, std::vector<ShaderLookUpID::Enum>> uniformGroups;
};



#endif // SHADER_LOADER_H
