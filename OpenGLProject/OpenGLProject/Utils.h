#ifndef UTILS_H
#define UTILS_H

#include <glfw3.h>
#include <string>

#include "Window.h"



namespace Utils
{
	// Register user data field to access it later from within static GLFWwindow callbacks (to avoid making variable global)
	void SetGLFWCallbackData(GLFWwindow* GLFWWindow, Window* data);

	// Access user data field from within static GLFWwindow callbacks
	Window* const GetGLFWCallbackData(GLFWwindow* GLFWWindow);

	std::string ReadFile(const std::string& path);
}



#endif // UTILS_H
