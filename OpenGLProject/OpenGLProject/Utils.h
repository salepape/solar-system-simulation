#ifndef UTILS_H
#define UTILS_H

#include <string>

struct GLFWwindow;
class Window;



class GLFWUtils
{
public:
	// Register user data field to access it later from within static GLFWwindow callbacks (to avoid making variable global)
	static void SetGLFWCallbackData(GLFWwindow* GLFWWindow, Window* data);

	// Access user data field from within static GLFWwindow callbacks
	static Window* GetGLFWCallbackData(GLFWwindow* GLFWWindow);
};

class FileUtils
{
public:
	static std::string ReadFile(const std::string& path);
};



#endif // UTILS_H
