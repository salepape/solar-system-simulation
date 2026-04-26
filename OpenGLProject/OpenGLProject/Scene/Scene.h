#ifndef SCENE_H
#define SCENE_H



// @todo - Find a way to keep a list of all SceneEntities and other renderables in this class instead of its children
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
};



#endif // SCENE_H
