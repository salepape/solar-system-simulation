#ifndef WINDOW_H
#define WINDOW_H

#include <glm/vec2.hpp>

#include <cstdint>
#include <string>

class PerspectiveCameraController;
struct GLFWwindow;



// @todo - Reduce exposure of most attributes once the event system is implemented
// Wrap a GLFW Window and an OpenGL Context
class Window
{
public:
	Window(const uint32_t inWidth, const uint32_t inHeight, const std::string& inTitle);
	~Window();

	GLFWwindow* GLFWWindow{ nullptr };

	// Needed unfortunately, to be able to access it from GLFWwindow scroll/cursor position callbacks
	// @todo - Implement a custom event system, to get rid of this pointer
	PerspectiveCameraController* cameraController{ nullptr };

	void SwapFrontAndBackBuffers() const;

	// Call all window callbacks associated with each queueing events
	void ProcessPendingEvents() const;

	float GetAspectRatio() const { return aspectRatio; }

	glm::vec2 ComputeCursorOffset(const double xPosition, const double yPosition);

	// Show/Hide the cursor and lock its motion to the window if hidden
	void SetCursorMode(const int modeValue) const;

private:
	uint32_t width{ 0 };
	uint32_t height{ 0 };
	std::string title;

	float aspectRatio{ 0.0f };

	glm::vec2 lastCursorPosition;
	bool isFirstMouseInput{ true };

	// Create the underlying GLFW Window and associate an OpenGL Context with it
	// Warning: any OpenGL command that would be run before the creation of the Window will fail (as the OpenGL Context creation is wrapped in GLFW Window class)
	void InitGLFWWindowAndOpenGLContext();

	// Destroy the underlying GLFW Window and its associated OpenGL Context
	// Warning: any OpenGL command that would be run after the destruction of the Window will fail (as the OpenGL Context destruction is wrapped in GLFW Window destructor)
	void ClearGLFWWindowAndOpenGLContext() const;

	// Set the OpenGL Context asssociated with the GLFW Window to be the current one used on the GPU rendering thread
	void MakeOpenGLContextCurrent() const;

	void Resize(const uint32_t newWidth, const uint32_t newHeight);
	void SetWindowResizeGLFWCallback() const;
};



#endif // WINDOW_H
