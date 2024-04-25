#ifndef SHADER_H
#define SHADER_H

#include <glm/mat4x4.hpp>
#include <iostream>
#include <string>
#include <unordered_map>



enum class ShaderProcessStage
{
	COMPILATION,
	LINKING,
};

// Class dealing with vertex and fragment GLSL shaders
class Shader
{
public:
	Shader(const std::string& vsPath, const std::string& fsPath);
	Shader(const Shader&) = delete;
	~Shader();

	// Needs to be called before we initialise a uniform of the shader
	void Enable() const;
	void Disable() const;

	// Utility uniform setters
	void setUniformBool(const std::string& name, const bool value);
	void setUniformInt(const std::string& name, const int value);
	void setUniformFloat(const std::string& name, const float value);
	void setUniformVec3(const std::string& name, const glm::vec3& value);
	void setUniformVec3(const std::string& name, const float x, const float y, const float z);
	void setUniformMat4(const std::string& name, const glm::mat4& mat);

	unsigned int GetRendererID() const { return rendererID; }

private:
	unsigned int rendererID{ 0 };

	std::unordered_map<std::string, int> uniformLocationCache;

	// Prevent glGetUniformLocation duplicate calls
	int GetUniformLocation(const std::string& name);

	// Store shader file (.vs or .fs) content into a string (used C code for better efficiency when it comes to read a file)
	std::string ParseShader(const std::string& path) const;

	// Create and compile a shader object
	unsigned int CreateShader(const unsigned int type, const std::string& source) const;

	// Create a program object on which we attach each created shader
	void CreateProgram(const unsigned int vsID, const unsigned int fsID);

	// Utility function to check object compilation/linking errors
	void CheckValidity(const unsigned int ID, const ShaderProcessStage shaderProcessStage) const;
};



#endif // SHADER_H