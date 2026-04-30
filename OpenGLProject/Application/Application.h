#ifndef APPLICATION_H
#define APPLICATION_H

#include <filesystem>
#include <memory>
#include <string>

class Window;



// Singleton class allowing a global access point for a unique Application instance
class Application
{
public:
	Application(const std::filesystem::path& inExecutablePath);

	// Virtual destructor (needed to handle any custom polymorphic deletion in child classes)
	virtual ~Application() = default;

	virtual void Run();

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

	// See what the simulation looks like when celestial bodies move slower/faster (won't be of any effect when simulation has been paused)
	void UpdateSpeed(const float inSpeedFactor);

	const std::filesystem::path& GetExecutablePath() const { return executablePath; }

protected:
	// Time [in seconds] between the last frame and the current one (used to reduce processing power differences between computers)
	float deltaTime{ 0.0f };

	virtual void SetUp() = 0;
	virtual void Tick();
	virtual void Refresh() = 0;

private:
	std::filesystem::path executablePath;

	// Unique Singleton instance defined in source file
	static Application* instance;

	// As Window destructor is called by unique_ptr at some point in Solar Application source file, 
	// and Window type is incomplete at this point (forward-declared), we have to implement a custom destructor
	struct WindowDeleter
	{
		void operator()(Window* ptr);
	};

	std::unique_ptr<Window, WindowDeleter> window;

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
