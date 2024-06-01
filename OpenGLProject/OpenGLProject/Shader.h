#ifndef SHADER_H
#define SHADER_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <unordered_map>



// Used to check validity of all shaders, and the program they are attached to
enum class ShaderProcessStage
{
	COMPILATION,
	LINKING,
};

// Class dealing with vertex and fragment GLSL shaders
class Shader
{
public:
	Shader(const std::string& inEntityName, const std::string& vsPath, const std::string& fsPath);
	~Shader();

	// Needs to be called before we initialise a uniform of the shader
	void Enable() const;
	void Disable() const;

	// Utility uniform setters
	void setUniformBool(const std::string& name, const bool value);
	void setUniformInt(const std::string& name, const int32_t value);
	void setUniformFloat(const std::string& name, const float value);
	void setUniformVec3(const std::string& name, const glm::vec3& value);
	void setUniformVec3(const std::string& name, const float x, const float y, const float z);
	void setUniformMat4(const std::string& name, const glm::mat4& mat);

	// Prevent glGetUniformLocation duplicate calls
	int32_t GetUniformLocation(const std::string& name);

	uint32_t GetRendererID() const { return rendererID; }
	const std::string& GetEntityName() const { return entityName; }

private:
	uint32_t rendererID{ 0 };
	std::string entityName;

	std::unordered_map<std::string, int32_t> uniformLocationCache;

	// Store shader file (.vs or .fs) content into a string (used C code for better efficiency when it comes to read a file)
	std::string ParseShader(const std::string& path) const;

	// Create and compile a shader object
	uint32_t CreateShader(const uint32_t type, const std::string& source) const;

	// Create a program object on which we attach each created shader
	void CreateProgram(const uint32_t vsID, const uint32_t fsID);

	// Utility function to check object compilation/linking errors
	void CheckValidity(const uint32_t ID, const ShaderProcessStage shaderProcessStage) const;
};



#endif // SHADER_H