#ifndef WINDOW_H
#define WINDOW_H

#include <glm/vec2.hpp>
#include <string>

class Controller;
struct GLFWwindow;



class Window
{
public:
	Window(const uint32_t inWidth, const uint32_t inHeight, const std::string& inTitle);

	GLFWwindow* GLFWWindow{ nullptr };

	// Needed unfortunately, to be able to access it from a GLFWwindow scroll and cursor position callbacks
	// @todo - Implement a custom event system, to get rid of this pointer?
	Controller* controller{ nullptr };

	// Tell GLFW that we want the window context to be the main one on the current thread
	int32_t MakeContextCurrent();

	// Swap font and back buffers (we sent to the screen the updated buffer)
	void SwapBuffers();

	// Check if any events are triggered, updates the window states and call the corresponding functions
	void PollEvents();

	// Clean up properly all previous allocated GLFW resources
	void FreeUpResources();

	float GetAspectRatio() const { return aspectRatio; }
	void UpdateDimensions(const uint32_t newWidth, const uint32_t newHeight);

	const glm::vec2& GetOffsetFromLastCursorPosition(const double xPosition, const double yPosition);

	// Make the cursor invisible to the player and allow movements even if cursor is theorically outside the window
	void SetCursorMode(const int modeValue);

private:
	uint32_t width{ 0 };
	uint32_t height{ 0 };
	const std::string title;

	float aspectRatio{ 0.0f };

	// Variables for mouse callback function
	bool firstMouseInput{ true };
	glm::vec2 lastCursorPositionCache;
	glm::vec2 offsetFromLastCursorPosition;

	GLFWwindow* const initGLFWWindow();

	// Adapt the viewport size whenever the window size has been changed (by OS or the user)
	void Callback_SetFramebufferResize();
};



#endif // WINDOW_H