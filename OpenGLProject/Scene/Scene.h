#ifndef SCENE_H
#define SCENE_H



// @todo - Do SPIKE for going full ECS instead of only Entity-Component Composition relationships
// @todo - Find a way to keep a list of all SceneEntities and other renderables in this class instead of its children. Update() should be defined in this class
// Contain entities that model the simulation. Be careful: shaders need to be all loaded first!
class Scene
{
public:
	Scene();

	// Virtual destructor (needed to handle any custom polymorphic deletion in child classes)
	virtual ~Scene() = default;

protected:
	virtual void Update(const float deltaTime) = 0;
	void Clear() const;
};



#endif // SCENE_H
