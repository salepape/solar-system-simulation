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
	return glfwGetKey(Application::GetInstance().GetWindow().GLFWWindow, keyCode) == GLFW_PRESS;
}

bool InputHandler::IsKeyReleased(const int keyCode)
{
	return glfwGetKey(Application::GetInstance().GetWindow().GLFWWindow, keyCode) == GLFW_RELEASE;
}

bool InputHandler::IsMouseButtonPressed(const int buttonCode)
{
	return glfwGetMouseButton(Application::GetInstance().GetWindow().GLFWWindow, buttonCode) == GLFW_PRESS;
}

