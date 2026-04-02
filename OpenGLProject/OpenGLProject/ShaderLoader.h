#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <vector>

class Shader;



// To be used to refer to any Shader instead of relying on raw strings (layer of security over the existence of LookUpIDs when instantiating or look-up functions)
namespace ShaderLookUpID
{
	// Enum elements do not correspond to GLSL Shader names but on which Scene Entity/Object Mesh they are applied to
	enum Enum
	{
		CELESTIAL_BODY = 0,
		SUN,
		BILLBOARD,
		BELT,
		MILKY_WAY,
		ORBIT,
		VISIBLE_BODY_RINGS,
		INFRARED_BODY_RINGS,
	};

	static const std::vector<Enum> All = { CELESTIAL_BODY, SUN, BILLBOARD, BELT, MILKY_WAY, ORBIT, VISIBLE_BODY_RINGS, INFRARED_BODY_RINGS, };

	static const Enum Get(const int index) { return All[index]; }
};

// @todo - Re-arrange so code is independant from the simulation ("Engine" code)
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