#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>
#include <glm/vec2.hpp>
#include <string>

class PerspectiveCameraController;
struct GLFWwindow;



// @todo - Reduce exposure of most attributes once the event system is implemented
class Window
{
public:
	Window(const uint32_t inWidth, const uint32_t inHeight, const std::string& inTitle);

	GLFWwindow* GLFWWindow{ nullptr };

	// Needed unfortunately, to be able to access it from GLFWwindow scroll/cursor position callbacks
	// @todo - Implement a custom event system, to get rid of this pointer
	PerspectiveCameraController* cameraController{ nullptr };

	// Tell GLFW that we want the context of the window to be the single one for the current thread
	void MakeContextCurrent() const;

	void SwapFrontAndBackBuffers() const;

	// Call all window callbacks associated with each queueing events
	void ProcessPendingEvents() const;

	void ClearResources() const;

	float GetAspectRatio() const { return aspectRatio; }
	void Resize(const uint32_t newWidth, const uint32_t newHeight);

	glm::vec2 ComputeCursorOffset(const double xPosition, const double yPosition);

	// Show/Hide the cursor and lock its motion to the window if hidden
	void SetCursorMode(const int modeValue) const;

private:
	uint32_t width{ 0 };
	uint32_t height{ 0 };
	std::string title;

	float aspectRatio{ 0.0f };

	bool isFirstMouseInput{ true };
	glm::vec2 lastCursorPosition;

	GLFWwindow* InitGLFWWindow() const;

	void Callback_DetectWindowResize() const;
};



#endif // WINDOW_H