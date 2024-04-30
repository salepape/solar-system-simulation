#ifndef WINDOW_H
#define WINDOW_H

#include <glm/vec2.hpp>
#include <iostream>
#include <string>

class Camera;
struct GLFWwindow;



class Window
{
public:
	Window(const unsigned int inWidth, const unsigned int inHeight, const std::string& inTitle);

	GLFWwindow* GLFWWindow{ nullptr };

	Camera* camera{ nullptr };

	// Tell GLFW that we want the window context to be the main one on the current thread
	int MakeContextCurrent();

	// Swap font and back buffers (we sent to the screen the updated buffer)
	void SwapBuffers();

	// Check whether a specific key is pressed / released and react accordingly
	void ProcessInput(Camera& camera);

	// Check if any events are triggered, updates the window states and call the corresponding functions
	void PollEvents();

	// Clean up properly all previous allocated GLFW resources
	void FreeUpResources();

	float GetAspectRatio() const;

private:
	int width{ 0 };
	int height{ 0 };
	const std::string title;

	// Variables for mouse callback function
	bool firstMouseInput{ true };
	glm::dvec2 lastCursorPosition;

	GLFWwindow* const initGLFWWindow();

	// Adapt the viewport size whenever the window size has been changed (by OS or the user)
	void SetFramebufferResizeCallback();

	// Detect if any mouse movement is made and react accordingly
	void SetCursorPositionCallback();

	// Detect if any mouse wheel movement is made and react accordingly
	void SetScrollCallback();

	// Make the cursor invisible to the player and allow movements even if cursor is theorically outside the window
	void SetInputMode();
};



#endif // WINDOW_H