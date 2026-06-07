#ifndef APPLICATION_H
#define APPLICATION_H

#include <filesystem>
#include <memory>
#include <string>

#include "Rendering/RenderQueue.h"

class Scene;
class Window;



// Singleton class allowing a global access point for a unique Application instance
class Application
{
public:
	Application(const std::filesystem::path& inExecutablePath, const std::string& inTitle);

	~Application();

	// Main Render Loop (run every frame)
	void Run();

	static Application& GetInstance();
	Window& GetWindow() const { return *window; }

	// Check if GLFW window has been instructed to close
	bool IsClosed() const;
	void Close() const;

	void Pause(const bool inIsPaused);

	bool IsLegendDisplayed() const { return isLegendDisplayed; }
	void DisplayLegend(const bool inIsLegendDisplayed) { isLegendDisplayed = inIsLegendDisplayed; }

	// Get time duration [in seconds] since the GLFW Window associated to the application has been created
	float GetElapsedTime() const;

	float GetSpeedFactor() const { return speedFactor; }
	bool IsPaused() const { return speedFactor == 0.0f; }
	bool IsMinSpeed() const { return speedFactor <= SPEED_MIN_THRESHOLD; }
	bool IsMaxSpeed() const { return speedFactor >= SPEED_MAX_THRESHOLD; }

	// See what the simulation run by the Application looks like when celestial bodies move slower/faster (won't be of any effect when paused)
	void UpdateSpeed(const float inSpeedFactor);

	const std::filesystem::path& GetExecutablePath() const { return executablePath; }

	RenderQueue& GetRenderQueue() { return renderQueue; }

	void AddScene(std::unique_ptr<Scene> inScene);

protected:
	// Time [in seconds] between the last frame and the current one (used to reduce processing power differences between computers)
	float deltaTime{ 0.0f };

	std::unique_ptr<Window> window;
	std::unique_ptr<Scene> scene;

	void Tick();
	void Refresh();

private:
	std::filesystem::path executablePath;

	// Unique Singleton instance defined in source file
	static Application* instance;

	RenderQueue renderQueue;

	// Time [in seconds] elapsed in Play mode since the GLFW Window associated to the application has been created
	float elapsedPlayTime{ 0.0f };
	// Time [in seconds] elapsed in Pause mode since the GLFW Window associated to the application has been created
	float elapsedPauseTime{ 0.0f };

	// Time cache [in seconds] to be able to compute Play delta time at next iteration
	float lastFrameElapsedPlayTime{ 0.0f };
	// Time cache [in seconds] to be able to compute Pause delta time at next iteration
	float lastFrameElapsedPauseTime{ 0.0f };

	// Factor which consistently increases/decreases the angle values travelled by each celestial body
	float speedFactor{ 1.0f };
	float cachedSpeedFactor{ 0.0f };
	constexpr static float SPEED_MIN_THRESHOLD = 0.125f;
	constexpr static float SPEED_MAX_THRESHOLD = 512.0f;

	bool isLegendDisplayed{ false };
};

#endif // APPLICATION_H
