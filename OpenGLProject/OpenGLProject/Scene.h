#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "Renderer.h"
#include "TextRenderer.h"

class Application;
class Controller;
class Window;



class Scene
{
public:
	Scene();

	// Create most of scene entities. Be careful: shaders need to be all loaded first!
	virtual void Update() = 0;

	// Free up all rendering resources
	void Clear();

	Application& runningApp;
	Window& openedWindow;

	Renderer renderer;
	TextRenderer textRenderer;

	std::shared_ptr<Controller> controller;
};



#endif // SCENE_H