#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <array>
#include <vector>

class Shader;



// To be used to refer to any Shader instead of relying on raw strings (layer of security over the existence of LookUpIDs when instantiating or look-up functions)
namespace ShaderLookUpID
{
	constexpr size_t Num = 5;

	// Enum elements do not correspond to GLSL Shader names but on which Scene Entity/Object Mesh they are applied to
	enum Enum
	{
		UNDEFINED = 0,
		DEFAULT,
		STAR,
		BILLBOARD,
		GALAXY_BACKGROUND,
		BELT,
	};

	constexpr std::array<Enum, Num> All = { DEFAULT, STAR, BILLBOARD, BELT, GALAXY_BACKGROUND, };

	constexpr Enum Get(const int index) { return All[index]; }
};

// Global access point to all Shaders that can be applied on Scene Entity/Objects of the simulation
class ShaderLibrary final
{
public:
	// Instantiate default Shaders for all Scene Entity/Objects of the simulation based on GLSL Vertex/Fragment Shaders, and common Uniform variables
	static void BuildDefaultShaders();

	static Shader& GetShader(const ShaderLookUpID::Enum inShaderName);

private:
	static std::vector<Shader> shaders;
};



#endif // SHADER_LOADER_H
