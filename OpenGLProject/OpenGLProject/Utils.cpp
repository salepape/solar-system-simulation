#include "Utils.h"



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
}