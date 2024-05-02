#include "Shader.h"

#include <fstream>
#include <glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <string>



Shader::Shader(const std::string& vsPath, const std::string& fsPath)
{
	const std::string& vsContent = ParseShader(vsPath);
	const std::string& fsContent = ParseShader(fsPath);

	const uint32_t vsID = CreateShader(GL_VERTEX_SHADER, vsContent);
	const uint32_t fsID = CreateShader(GL_FRAGMENT_SHADER, fsContent);

	CreateProgram(vsID, fsID);
}

Shader::~Shader()
{
	glDeleteProgram(rendererID);
}

std::string Shader::ParseShader(const std::string& path) const
{
	std::ifstream shaderFile(path);
	if (!shaderFile)
	{
		std::cout << "ERROR::SHADER - File not successfully read" << std::endl;
		return std::string();
	}

	// Read file's buffer content into stream
	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();

	shaderFile.close();

	return shaderStream.str();
}

uint32_t Shader::CreateShader(const uint32_t type, const std::string& content) const
{
	const uint32_t shaderID = glCreateShader(type);
	const char* contentRawPointer = content.c_str();
	glShaderSource(shaderID, 1, &contentRawPointer, nullptr);

	glCompileShader(shaderID);

	CheckValidity(shaderID, ShaderProcessStage::COMPILATION);

	return shaderID;
}

void Shader::CreateProgram(const uint32_t vertexShaderID, const uint32_t fragmentShaderID)
{
	rendererID = glCreateProgram();

	glAttachShader(rendererID, vertexShaderID);
	glAttachShader(rendererID, fragmentShaderID);

	glLinkProgram(rendererID);
	glValidateProgram(rendererID);

	CheckValidity(rendererID, ShaderProcessStage::LINKING);

	// Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
	{
		return uniformLocationCache[name];
	}

	const int32_t uniformLocation = glGetUniformLocation(rendererID, name.c_str());
	if (uniformLocation == -1)
	{
		std::cout << "ERROR::SHADER - uniform " << name << " doesn't exist!" << std::endl;
	}

	uniformLocationCache[name] = uniformLocation;

	return uniformLocation;
}

void Shader::Enable() const
{
	glUseProgram(rendererID);
}

void Shader::Disable() const
{
	glUseProgram(0);
}

void Shader::setUniformBool(const std::string& name, const bool value)
{
	glUniform1i(GetUniformLocation(name), static_cast<int32_t>(value));
}

void Shader::setUniformInt(const std::string& name, const int32_t value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::setUniformFloat(const std::string& name, const float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::setUniformVec3(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setUniformVec3(const std::string& name, const float x, const float y, const float z)
{
	glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::CheckValidity(const uint32_t ID, const ShaderProcessStage processStage) const
{
	switch (processStage)
	{
	case ShaderProcessStage::COMPILATION:
	{
		int32_t compileStatus;
		glGetShaderiv(ID, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == false)
		{
			int32_t logLength = 0;
			glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &logLength);

			// We can't stack-allocate a char* and std::string is const, but we can use std::vector instead of heap-allocate
			std::vector<char> errorMessage(logLength);
			glGetShaderInfoLog(ID, logLength, &logLength, errorMessage.data());

			std::cout << "ERROR::SHADER - Error: " << errorMessage.data() << std::endl;
		}

		break;
	}
	case ShaderProcessStage::LINKING:
	{
		int32_t linkStatus;
		glGetProgramiv(ID, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == false)
		{
			int32_t logLength = 0;
			glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLength);

			// We can't stack-allocate a char* and std::string is const, but we can use std::vector instead of heap-allocate
			std::vector<char> errorMessage(logLength);
			glGetProgramInfoLog(ID, logLength, &logLength, errorMessage.data());

			std::cout << "ERROR::PROGRAM - Error: " << errorMessage.data() << std::endl;
		}

		break;
	}
	}
}