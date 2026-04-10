#ifndef SHADER_H
#define SHADER_H

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <unordered_map>

#include "ShaderLoader.h"


// Used to check validity of GLSL Shaders at all stages of their building process, and the GLSL Program they are attached to
enum class ShaderProcessStage
{
	COMPILATION,
	LINKING,
};

// Wrapper of a combination of GLSL Vertex/Fragment Shaders associated with a Scene Entity/Object, that handles its creation, compilation, modification, and deletion
class Shader
{
public:
	Shader(const ShaderLookUpID::Enum inEntityName, const std::string& vsPath, const std::string& fsPath);
	~Shader();

	// Needs to be called before we initialise a uniform defined in the shader
	void Enable() const;
	void Disable() const;

	// Utility uniform setters
	void setUniformBool(const std::string& name, const bool value) const;
	void setUniformInt(const std::string& name, const int32_t value) const;
	void setUniformFloat(const std::string& name, const float value) const;
	void setUniformVec3(const std::string& name, const glm::vec3& value) const;
	void setUniformVec3(const std::string& name, const float x, const float y, const float z) const;
	void setUniformMat4(const std::string& name, const glm::mat4& mat) const;

	// Return whether the Uniform is required for the Shader and perform caching of its location if so. Should always be called prior to attempt setting a Uniform 
	bool IsUniformRequired(const std::string& name);

	uint32_t GetRendererID() const { return rendererID; }
	const ShaderLookUpID::Enum GetShaderLookUpID() const { return lookUpID; }

private:
	uint32_t rendererID{ 0 };
	ShaderLookUpID::Enum lookUpID;

	// Prevent glGetUniformLocation() duplicate calls while also guaranteeing the Uniform is required by the Shader
	std::unordered_map<std::string, int32_t> uniformLocationCache;

	// Create and compile a shader object
	uint32_t CreateShader(const uint32_t type, const std::string& source) const;

	// Create a program object on which we attach each created shader
	void CreateProgram(const uint32_t vsID, const uint32_t fsID);

	// Utility function to check object compilation/linking errors
	void CheckValidity(const uint32_t ID, const ShaderProcessStage shaderProcessStage) const;

	// Should only be called internally, and always after a IsUniformRequired() call so the local Uniform cache is set up appropriately
	int32_t GetUniformLocation(const std::string& name) const;
};



#endif // SHADER_H