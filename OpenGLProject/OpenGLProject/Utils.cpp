#include "Utils.h"

#include <filesystem>
#include <fstream>


namespace Utils
{
	void SetGLFWCallbackData(GLFWwindow* GLFWWindow, Window* data)
	{
		glfwSetWindowUserPointer(GLFWWindow, static_cast<void*>(data));
	}

	Window* const GetGLFWCallbackData(GLFWwindow* GLFWWindow)
	{
		return static_cast<Window*>(glfwGetWindowUserPointer(GLFWWindow));
	}

	std::string ReadFile(const std::string & path)
	{
		std::ifstream fileStream(path, std::ios::in | std::ios::binary);
		const uintmax_t fileSizeInBytes = std::filesystem::file_size(path);

		std::string fileContent(static_cast<uint32_t>(fileSizeInBytes), '\0');
		fileStream.read(fileContent.data(), fileSizeInBytes);

		return fileContent;
	}
}