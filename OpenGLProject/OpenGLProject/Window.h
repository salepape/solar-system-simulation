#ifndef WINDOW_H
#define WINDOW_H

#include <glm/vec2.hpp>
#include <memory>
#include <string>

#include "Controller.h"

struct GLFWwindow;



// @todo - Reduce exposure of most attributes once the event system is implemented
class Window
{
public:
	Window(const uint32_t inWidth, const uint32_t inHeight, const std::string& inTitle);

	GLFWwindow* GLFWWindow{ nullptr };

	// Needed unfortunately, to be able to access it from a GLFWwindow scroll and cursor position callbacks
	// @todo - Implement a custom event system, to get rid of this pointer?
	std::shared_ptr<Controller> controller;

	// Tell GLFW that we want the context of the window to be the current single one for the current thread
	int32_t MakeContextCurrent();

	void SwapFrontAndBackBuffers();

	// Call all window callbacks associated with each queueing events
	void ProcessPendingEvents();

	void ClearResources();

	float GetAspectRatio() const { return aspectRatio; }
	void Resize(const uint32_t newWidth, const uint32_t newHeight);

	const glm::vec2 ComputeCursorOffset(const double xPosition, const double yPosition);

	// Show/Hide the cursor and lock its motion to the window if hidden
	void SetCursorMode(const int modeValue);

private:
	uint32_t width{ 0 };
	uint32_t height{ 0 };
	const std::string title;

	float aspectRatio{ 0.0f };

	bool isFirstMouseInput{ true };
	glm::vec2 lastCursorPosition;

	GLFWwindow* const InitGLFWWindow();

	void Callback_DetectWindowResize();
};



#endif // WINDOW_H