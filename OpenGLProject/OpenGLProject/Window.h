#ifndef WINDOW_H
#define WINDOW_H

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

	// Check if GLFW has been instructed to close
	bool DoNotClose();

	void UpdateFrames();
	float GetFrameRate() const;

	// Check if any events are triggered, updates the window states and call the corresponding functions
	void PollEvents();

	// Clean up properly all previous allocated GLFW resources
	void FreeUpResources();

	bool IsSimuPaused() const { return simuPaused; }

private:
	int width{ 0 };
	int height{ 0 };
	const std::string title;

	// @todo - All variables below plus linked code to be moved in Engine/UserInputHandler (KeyInput + MouseInput) classes?

	// Variables for time related to frames
	float currentFrame{ 0.0f };
	float deltaTime{ 0.0f };
	float lastFrame{ 0.0f };
	bool simuPaused{ false };
	float simuSpeedFactor{ 1.0f };

	// Variables for mouse callback function
	bool firstMouseInput{ true };
	double lastXPos{ 0.0f };
	double lastYPos{ 0.0f };

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