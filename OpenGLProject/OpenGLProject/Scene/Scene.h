#ifndef SCENE_H
#define SCENE_H

#include "Rendering/Renderer.h"
#include "Rendering/TextRenderer.h"



// @todo - Find a way to keep a lisr of all SceneEntities and other renderable in this class
// Contain entities that model the simulation. Be careful: shaders need to be all loaded first!
class Scene
{
public:
	Scene();

	// Virtual destructor (needed to handle any custom polymorphic deletion in child classes)
	virtual ~Scene() = default;

protected:
	virtual void Update() = 0;
	void Clear() const;

	Renderer renderer;
	TextRenderer textRenderer;
};



#endif // SCENE_H