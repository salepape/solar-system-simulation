#ifndef SCENE_H
#define SCENE_H

#include "Renderer.h"
#include "TextRenderer.h"

class Application;
class Window;



// Contain entities that model the simulation. Be careful: shaders need to be all loaded first!
class Scene
{
public:
	Scene();

	// Virtual destructor (needed to handle any custom polymorphic deletion in child classes)
	virtual ~Scene() = default;

	virtual void Update() = 0;
	void Clear() const;

	// @todo - Avoid these references as member variables
	// Cache Application and Window so we don't need to get their instance each frame in the Render loop
	Application& runningApp;
	Window& openWindow;

	Renderer renderer;
	TextRenderer textRenderer;
};



#endif // SCENE_H