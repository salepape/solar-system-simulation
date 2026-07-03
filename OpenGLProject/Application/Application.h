#ifndef APPLICATION_H
#define APPLICATION_H

#include <filesystem>
#include <memory>
#include <string>

class Scene;
class Window;



// Singleton class allowing a global access point for a unique Application instance
class Application
{
public:
	void SetParameters(const std::filesystem::path& inExecutablePath, const std::string& inTitle);
	~Application();

	static Application& GetInstance();
	Window& GetWindow() const { return *window; }

	void Run() const;

	// Check if GLFW window has been instructed to close
	bool IsClosed() const;
	void Close() const;

	void Pause(const bool inIsPaused);

	bool IsLegendDisplayed() const { return isLegendDisplayed; }
	void DisplayLegend(const bool inIsLegendDisplayed) { isLegendDisplayed = inIsLegendDisplayed; }

	float GetSpeedFactor() const { return speedFactor; }
	bool IsPaused() const { return speedFactor == 0.0f; }
	bool IsMinSpeed() const { return speedFactor <= SPEED_MIN_THRESHOLD; }
	bool IsMaxSpeed() const { return speedFactor >= SPEED_MAX_THRESHOLD; }

	// See what the simulation run by the Application looks like when celestial bodies move slower/faster (won't be of any effect when paused)
	void UpdateSpeed(const float inSpeedFactor);

	const std::filesystem::path& GetExecutablePath() const { return executablePath; }

	void SetScene(std::unique_ptr<Scene> scene);

protected:
	std::unique_ptr<Window> window;

private:
	std::filesystem::path executablePath;

	// Unique Singleton instance defined in source file
	static Application* instance;

	// Factor which consistently increases/decreases the angle values travelled by each celestial body
	float speedFactor{ 1.0f };
	float cachedSpeedFactor{ 0.0f };
	constexpr static float SPEED_MIN_THRESHOLD = 0.125f;
	constexpr static float SPEED_MAX_THRESHOLD = 512.0f;

	bool isLegendDisplayed{ false };
};



#endif // APPLICATION_H
