#include "Utils.h"

#include <filesystem>
#include <fstream>
#include <glfw3.h>

#include "Window.h"



void GLFWUtils::SetGLFWCallbackData(GLFWwindow* GLFWWindow, Window* data)
{
	glfwSetWindowUserPointer(GLFWWindow, static_cast<void*>(data));
}

Window* GLFWUtils::GetGLFWCallbackData(GLFWwindow* GLFWWindow)
{
	return static_cast<Window*>(glfwGetWindowUserPointer(GLFWWindow));
}

std::string FileUtils::ReadFile(const std::string & path)
{
	std::ifstream fileStream(path, std::ios::in | std::ios::binary);
	const uintmax_t fileSizeInBytes = std::filesystem::file_size(path);

	std::string fileContent(static_cast<uint32_t>(fileSizeInBytes), '\0');
	fileStream.read(fileContent.data(), fileSizeInBytes);

	return fileContent;
}
