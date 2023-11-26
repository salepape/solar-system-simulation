#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>


enum class ObjectType
{
	SHADER,
	PROGRAM,
};

// Class dealing with vertex and fragment GLSL shaders
class Shader
{
private:
	unsigned int rendererID{ 0 };
	std::unordered_map<std::string, int> uniformLocationCache;

	// Store shader file (.vs or .fs) content into a string (used C code for better efficiency when it comes to read a file)
	std::string ParseShader(const std::string& path);

	// Create and compile a shader object
	unsigned int CreateShader(const unsigned int type, const std::string& source);

	// Create a program object on which we attach shaders
	void CreateProgram(const unsigned int vsID, const unsigned int fsID);

	// Utility function to check object compilation/linking errors
	void CheckValidity(const unsigned int ID, const ObjectType errorCode);

	int GetUniformLocation(const std::string& name);

public:
	Shader(const std::string& vsPath, const std::string& fsPath);
	~Shader();

	void Enable() const;
	void Disable() const;

	// Utility uniform setters
	void setUniformBool(const std::string& name, const bool value);
	void setUniformInt(const std::string& name, const int value);
	void setUniformFloat(const std::string& name, const float value);
	void setUniformVec3(const std::string& name, const glm::vec3& value);
	void setUniformVec3(const std::string& name, const float x, const float y, const float z);
	void setUniformMat4(const std::string& name, const glm::mat4& mat);
};



#endif // SHADER_H