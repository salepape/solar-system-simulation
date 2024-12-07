#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "Renderer.h"
#include "TextRenderer.h"

class Application;
class Controller;
class Window;



// Contain entities that model the simulation. Be careful: shaders need to be all loaded first!
class Scene
{
public:
	Scene();

	virtual void Update() = 0;
	void Clear();

	// Cache Application and Window so we don't need to get their instance each frame in the Render loop
	Application& runningApp;
	Window& openWindow;

	Renderer renderer;
	TextRenderer textRenderer;

	std::shared_ptr<Controller> controller;
};



#endif // SCENE_H