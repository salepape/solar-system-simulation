#ifndef APPLICATION_H
#define APPLICATION_H

class Window;



// 1 second corresponds to 1 Earth day in the simulation
class Application
{
public:
	Application();
	~Application();

	static Application& GetInstance() { return *instance; }
	Window& GetWindow() const { return *window; }

	void Run();

	// Check if GLFW window has been instructed to close
	bool IsNotClosed() const;
	void Close() const;

	bool IsPaused() const { return isPaused; }
	void Pause(const bool inIsPaused);

	bool IsLegend() const { return isLegend; }
	void DisplayLegend(const bool inIsLegend) { isLegend = inIsLegend; }

	// See what the simulation looks like with celestial body slower/faster movements (does not keep body positions between different speed simulations)
	// @todo - Jumps in the simulation are due to the Model matrix of each body being computed from elapsed time and not delta time
	void ChangeSpeed(const float inSpeedFactor);

	double GetTime();

private:
	static Application* instance;
	Window* window{ nullptr };

	// Time elapsed since GLFW initialisation [in seconds]
	float elapsedTime{ 0.0f };
	float lastFrameElapsedTime{ 0.0f };
	// Compute delta time between last frame and current one in order to reduce differences between computer processing powers
	float deltaTime{ 0.0f };
	bool isPaused{ false };
	// Factor which consistently increases/decreases the angle values travelled by the celestial bodies since the simulation started
	float speedFactor{ 1.0f };

	bool isLegend{ false };

	// Coordinate system based on constant Solar position.
	// @todo - Create a specific SolarSystem solution to use the generic "OpenGL engine" code
	void SimulateSolarSystem();

	void Tick();
	float GetElapsedTime() const;
};

#endif // APPLICATION_H