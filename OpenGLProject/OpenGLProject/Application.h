#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "Window.h"



class Application
{
public:
	Application();
	~Application() = default;

	virtual void SetUp() = 0;
	virtual void Run();
	void Tick();
	virtual void Refresh() = 0;
	virtual void Terminate();

	float elapsedTime{ 0.0f };

	// Factor which consistently increases/decreases the angle values travelled by the celestial bodies since the simulation started
	float speedFactor{ 1.0f };

	// Compute delta time between last frame and current one in order to reduce differences between computer processing powers
	float deltaTime{ 0.0f };

	static Application& GetInstance() { return *instance; }
	Window& GetWindow() const { return *window; }

	// Check if GLFW window has been instructed to close
	bool IsClosed() const;
	void Close() const;

	bool IsPaused() const { return isPaused; }
	void Pause(const bool inIsPaused);

	bool IsLegendDisplayed() const { return isLegendDisplayed; }
	void DisplayLegend(const bool inIsLegendDisplayed) { isLegendDisplayed = inIsLegendDisplayed; }

	double GetTime();

	// @todo - Jumps in the simulation are due to the Model matrix of each body being computed from elapsed time and not delta time. Worth solving it? Seems complex...
	// See what the simulation looks like with celestial body slower/faster movements (does not keep body positions between different speed simulations)
	void UpdateSpeed(const float inSpeedFactor);

private:
	static Application* instance;
	std::unique_ptr<Window> window;

	// Time elapsed since GLFW initialisation [in seconds]
	float lastFrameElapsedTime{ 0.0f };
	bool isPaused{ false };
	bool isLegendDisplayed{ false };
};

#endif // APPLICATION_H