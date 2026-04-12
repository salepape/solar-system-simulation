#include "Shader.h"

#include <cassert>
#include <iostream>
#include <glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Utils.h"



Shader::Shader(const ShaderLookUpID::Enum inLookUpID, const std::string& vsPath, const std::string& fsPath) :
	lookUpID(inLookUpID)
{
	const std::string vsContent(FileUtils::ReadFile(vsPath));
	const std::string fsContent(FileUtils::ReadFile(fsPath));

	const uint32_t vsID = CreateShader(GL_VERTEX_SHADER, vsContent);
	const uint32_t fsID = CreateShader(GL_FRAGMENT_SHADER, fsContent);

	CreateProgram(vsID, fsID);
}

Shader::~Shader()
{
	glDeleteProgram(rendererID);
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

int32_t Shader::GetUniformLocation(const std::string& name) const
{
	if (uniformLocationCache.find(name) == uniformLocationCache.end())
	{
		std::cout << "ERROR::SHADER - Attempting to set Uniform " << name << " for Shader " << ShaderLookUpID::Get(lookUpID) << ", but Uniform has not been cached beforehand.\nCheck if you can do it at initialisation time.\n" << std::endl;
		assert(false);
	}

	return uniformLocationCache.at(name);
}

bool Shader::IsUniformRequired(const std::string& name)
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
	{
		return true;
	}

	const int32_t uniformLocation = glGetUniformLocation(rendererID, name.c_str());
	if (uniformLocation != -1)
	{
		uniformLocationCache[name] = uniformLocation;
		return true;
	}
	else
	{
		return false;
	}
}

void Shader::Enable() const
{
	glUseProgram(rendererID);
}

void Shader::Disable() const
{
	glUseProgram(0);
}

void Shader::setUniformBool(const std::string& name, const bool value) const
{
	glUniform1i(GetUniformLocation(name), static_cast<int32_t>(value));
}

void Shader::setUniformInt(const std::string& name, const int32_t value) const
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::setUniformFloat(const std::string& name, const float value) const
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::setUniformVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setUniformVec3(const std::string& name, const float x, const float y, const float z) const
{
	glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& mat) const
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
			if (logLength <= 2)
			{
				std::cout << "ERROR::SHADER for Shader '" << ShaderLookUpID::Get(lookUpID) << "'\nReason: No specific error message returned - Log length too short.\n" << std::endl;
				assert(false);
			}

			// We cannot stack-allocate a char*, and std::array cannot be used anyway since logLength is not constexpr, so we heap-allocate via std::vector, but reserving needed memory at creation time
			std::vector<char> errorMessage(logLength);
			glGetShaderInfoLog(ID, logLength, &logLength, errorMessage.data());
			if (errorMessage.data() == nullptr)
			{
				std::cout << "ERROR::SHADER for Shader '" << ShaderLookUpID::Get(lookUpID) << "'\nReason: no specific error message returned by glGetShaderInfoLog().\n" << std::endl;
				assert(false);
			}
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
			if (logLength <= 2)
			{
				std::cout << "ERROR::PROGRAM for Shader '" << ShaderLookUpID::Get(lookUpID) << "'\nReason: No specific error message returned - Log length too short.\n" << std::endl;
				assert(false);
			}

			// We cannot stack-allocate a char*, and std::array cannot be used anyway since logLength is not constexpr, so we heap-allocate via std::vector, but reserving needed memory at creation time
			std::vector<char> errorMessage(logLength);
			glGetProgramInfoLog(ID, logLength, &logLength, errorMessage.data());
			if (errorMessage.data() == nullptr)
			{
				std::cout << "ERROR::PROGRAM for Shader '" << ShaderLookUpID::Get(lookUpID) << "'\nReason: no specific error message returned by glGetProgramInfoLog().\n" << std::endl;
				assert(false);
			}
		}

		break;
	}
	}
}