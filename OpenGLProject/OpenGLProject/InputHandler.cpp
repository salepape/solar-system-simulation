#include "InputHandler.h"

#include <glfw3.h>

#include "Application.h"
#include "Window.h"

InputHandler* InputHandler::instance = nullptr;



InputHandler& InputHandler::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new InputHandler();
	}

	return *instance;
}

bool InputHandler::IsKeyPressed(const int keyCode)
{
	Window& currentWindow = Application::GetInstance().GetWindow();
	return glfwGetKey(currentWindow.GLFWWindow, keyCode) == GLFW_PRESS;
}

bool InputHandler::IsKeyReleased(const int keyCode)
{
	Window& currentWindow = Application::GetInstance().GetWindow();
	return glfwGetKey(currentWindow.GLFWWindow, keyCode) == GLFW_RELEASE;
}

bool InputHandler::IsMouseButtonPressed(const int buttonCode)
{
	Window& currentWindow = Application::GetInstance().GetWindow();
	return glfwGetMouseButton(currentWindow.GLFWWindow, buttonCode) == GLFW_PRESS;
}

