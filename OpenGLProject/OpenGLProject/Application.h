#ifndef APPLICATION_H
#define APPLICATION_H

class Window;



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

	void ChangeSpeed(const float inSpeedFactor) { speedFactor *= inSpeedFactor; }

	double GetTime();

private:
	static Application* instance;
	Window* window{ nullptr };

	// Variables for time related to frames
	float currentFrame{ 0.0f };
	float lastFrame{ 0.0f };
	float deltaTime{ 0.0f };
	bool isPaused{ false };
	float speedFactor{ 1.0f };

	bool isLegend{ false };

	// @todo - Create a specific SolarSystem solution to use the generic "OpenGL engine" code
	void SimulateSolarSystem();

	void UpdateFrames();
	float GetFrameRate() const;
};

#endif // APPLICATION_H