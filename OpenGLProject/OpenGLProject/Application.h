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

private:
	static Application* instance;
	Window* window{ nullptr };

	// @todo - Create a specific SolarSystem solution to use the generic "OpenGL engine" code
	void SimulateSolarSystem();
};

#endif // APPLICATION_H