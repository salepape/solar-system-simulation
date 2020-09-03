#include "ShaderProgram.h"



ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath, const char* geometryPath) : rendererID(0)
{
	isGeom = (geometryPath != nullptr) ? true : false;

	// Store .vs and .fs shader files into string
	std::string vertexSource = ParseShader(vertexPath);
	std::string fragmentSource = ParseShader(fragmentPath);
	std::string geometrySource;
	if (isGeom)
		geometrySource = ParseShader(geometryPath);

	// Create and compile shaders object
	unsigned int vsID = CreateShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fsID = CreateShader(GL_FRAGMENT_SHADER, fragmentSource);
	unsigned int gsID = -1;
	if (isGeom)
		gsID = CreateShader(GL_GEOMETRY_SHADER, geometrySource);

	// Create complete shader program object
	CreateShaderProgram(vsID, fsID, gsID);
}

std::string ShaderProgram::ParseShader(const char* path)
{
	// Retrieve the source code from file path
	std::string code;
	std::ifstream shaderFile;

	// Ensure ifstream objects can throw exceptions
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open file
		shaderFile.open(path);

		// Read file's buffer content into streams
		std::stringstream vShaderStream;
		vShaderStream << shaderFile.rdbuf();

		// Close file handlers
		shaderFile.close();

		// Convert stream into string
		code = vShaderStream.str();
	}
	catch (std::ifstream::failure&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return code;
}

unsigned int ShaderProgram::CreateShader(unsigned int type, const std::string& source)
{
	unsigned int shaderID = glCreateShader(type);
	const char* shaderCode = source.c_str();

	glShaderSource(shaderID, 1, &shaderCode, nullptr);
	glCompileShader(shaderID);

	checkCompileErrors(shaderID, "SHADER");

	return shaderID;
}

void ShaderProgram::CreateShaderProgram(unsigned int vsID, unsigned int fsID, unsigned int gsID)
{
	// Create program object
	rendererID = glCreateProgram();

	// Attach vertex and fragment shaders objects to it
	glAttachShader(rendererID, vsID);
	glAttachShader(rendererID, fsID);
	if (isGeom)
		glAttachShader(rendererID, gsID);

	// Link program to the current ID
	glLinkProgram(rendererID);
	checkCompileErrors(rendererID, "PROGRAM");		// or glValidateProgram(program); ?

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vsID);
	glDeleteShader(fsID);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(rendererID);
}

void ShaderProgram::Enable() const
{
	glUseProgram(rendererID);
}

void ShaderProgram::Disable() const
{
	glUseProgram(0);
}

void ShaderProgram::setBool(const std::string &name, bool value)
{
	glUniform1i(GetUniformLocation(name), (int)value);
}

void ShaderProgram::setInt(const std::string &name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::setFloat(const std::string &name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void ShaderProgram::setVec2(const std::string &name, const glm::vec2 &value)
{
	glUniform2fv(GetUniformLocation(name), 1, &value[0]);
}

void ShaderProgram::setVec2(const std::string &name, float x, float y)
{
	glUniform2f(GetUniformLocation(name), x, y);
}

void ShaderProgram::setVec3(const std::string &name, const glm::vec3 &value)
{
	glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}

void ShaderProgram::setVec3(const std::string &name, float x, float y, float z)
{
	glUniform3f(GetUniformLocation(name), x, y, z);
}

void ShaderProgram::setVec4(const std::string &name, const glm::vec4 &value)
{
	glUniform4fv(GetUniformLocation(name), 1, &value[0]);
}

void ShaderProgram::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void ShaderProgram::setMat2(const std::string &name, const glm::mat2 &mat)
{
	glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat3(const std::string &name, const glm::mat3 &mat)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat4(const std::string &name, const glm::mat4 &mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n" << std::endl;
		}
	}
}

int ShaderProgram::GetUniformLocation(const std::string& name)
{
	// Prevent glGetUniformLocation duplicate calls
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
		return uniformLocationCache[name];

	int location = glGetUniformLocation(rendererID, name.c_str());
	//if (location == -1)
	//	std::cout << "ERROR::SHADER: uniform " << name << " doesn't exist !" << std::endl;

	uniformLocationCache[name] = location;

	return location;
}