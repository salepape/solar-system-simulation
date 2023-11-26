#include "Shader.h"



Shader::Shader(const std::string& vsPath, const std::string& fsPath) :
	rendererID(0)
{
	const std::string vsCode = ParseShader(vsPath);
	const std::string fsCode = ParseShader(fsPath);

	const unsigned int vsID = CreateShader(GL_VERTEX_SHADER, vsCode);
	const unsigned int fsID = CreateShader(GL_FRAGMENT_SHADER, fsCode);

	CreateProgram(vsID, fsID);
}

Shader::~Shader()
{
	glDeleteProgram(rendererID);
}

std::string Shader::ParseShader(const std::string& path)
{
	// Retrieve the source code from file path
	std::string shaderCode;
	std::ifstream shaderFile;

	// Ensure ifstream objects can throw exceptions
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open file
		shaderFile.open(path);

		// Read file's buffer content into streams
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		// Close file handlers
		shaderFile.close();

		// Convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure&)
	{
		std::cout << "ERROR::SHADER - File not successfully read" << std::endl;
	}

	return shaderCode;
}

unsigned int Shader::CreateShader(const unsigned int type, const std::string& content)
{
	const unsigned int shaderID = glCreateShader(type);
	const char* contentRawPointer = content.c_str();

	glShaderSource(shaderID, 1, &contentRawPointer, nullptr);
	glCompileShader(shaderID);

	CheckValidity(shaderID, ObjectType::SHADER);

	return shaderID;
}

void Shader::CreateProgram(const unsigned int vsID, const unsigned int fsID)
{
	rendererID = glCreateProgram();

	glAttachShader(rendererID, vsID);
	glAttachShader(rendererID, fsID);

	glLinkProgram(rendererID);
	glValidateProgram(rendererID);

	CheckValidity(rendererID, ObjectType::PROGRAM);

	// Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vsID);
	glDeleteShader(fsID);
}

int Shader::GetUniformLocation(const std::string& name)
{
	// Prevent glGetUniformLocation duplicate calls
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
	{
		return uniformLocationCache[name];
	}

	const int location = glGetUniformLocation(rendererID, name.c_str());
	if (location == -1)
	{
		std::cout << "ERROR::SHADER - uniform " << name << " doesn't exist !" << std::endl;
	}

	uniformLocationCache[name] = location;

	return location;
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
	glUniform1i(GetUniformLocation(name), static_cast<int>(value));
}

void Shader::setUniformInt(const std::string& name, const int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::setUniformFloat(const std::string& name, const float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::setUniformVec3(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::setUniformVec3(const std::string& name, const float x, const float y, const float z)
{
	glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::CheckValidity(const unsigned int ID, const ObjectType objectType)
{
	switch (objectType)
	{
	case ObjectType::SHADER:
	{
		int isCompiled;
		glGetShaderiv(ID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			int length = 0;
			glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);

			// Need to heap allocate since we can't stack allocate a char*, alloca is dangerous and std::string is const
			char* errorMessage = new char(length);
			glGetShaderInfoLog(ID, length, &length, errorMessage);

			std::cout << "ERROR::SHADER - Error: " << errorMessage << std::endl;

			delete errorMessage;
		}
	}
	case ObjectType::PROGRAM:
	{
		int isLinked;
		glGetProgramiv(ID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			int length = 0;
			glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);

			// Need to heap allocate since we can't stack allocate a char*, alloca is dangerous and std::string is const
			char* errorMessage = new char(length);
			glGetProgramInfoLog(ID, length, &length, errorMessage);

			std::cout << "ERROR::PROGRAM - Error: " << errorMessage << std::endl;

			delete errorMessage;
		}
	}
	}
}