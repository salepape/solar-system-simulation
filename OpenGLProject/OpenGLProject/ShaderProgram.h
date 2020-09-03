#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>



// Class dealing with a complete shader program (vertex and fragment GLSL shaders in a program)
class ShaderProgram
{
private:
	unsigned int rendererID;
	std::unordered_map<std::string, int> uniformLocationCache;
	bool isGeom;

	std::string ParseShader(const char* path);
	unsigned int CreateShader(unsigned int type, const std::string& source);
	void CreateShaderProgram(unsigned int vsID, unsigned int fsID, unsigned int gsID);

	// Utility function for checking shader compilation / linking errors
	void checkCompileErrors(GLuint shader, std::string type);
	int GetUniformLocation(const std::string& name);

public:
	// Constructor generating the shader on the fly
	ShaderProgram(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	~ShaderProgram();

	// Activate the shader
	void Enable() const;
	void Disable() const;

	// Utility uniform functions
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setVec2(const std::string& name, const glm::vec2 &value);
	void setVec2(const std::string& name, float x, float y);
	void setVec3(const std::string& name, const glm::vec3 &value);
	void setVec3(const std::string& name, float x, float y, float z);
	void setVec4(const std::string& name, const glm::vec4 &value);
	void setVec4(const std::string& name, float x, float y, float z, float w);
	void setMat2(const std::string& name, const glm::mat2 &mat);
	void setMat3(const std::string& name, const glm::mat3 &mat);
	void setMat4(const std::string& name, const glm::mat4 &mat);
};

#endif