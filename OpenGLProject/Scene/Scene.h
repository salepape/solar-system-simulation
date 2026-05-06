#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <cstddef>	// std::size_t
#include <vector>

class SceneEntity;



// @todo - Do SPIKE for going full ECS instead of only Entity-Component Composition relationships
// @todo - Find a way to keep a list of all SceneEntities and other renderables in this class instead of its children. Update() should be defined in this class
// Contain entities that model the simulation. Be careful: shaders need to be all loaded first!
class Scene
{
public:
	Scene();

	// Virtual destructor (needed to handle any custom polymorphic deletion in child classes)
	virtual ~Scene();

	virtual void Update(const float /*deltaTime*/);

	// Re-allocate the total number of bytes needed for the vector after adding the provided capacity
	void AllocateMemory(const size_t numOfBytes);

protected:
	void AddEntity(std::unique_ptr<SceneEntity> inEntity);

private:
	std::vector<std::unique_ptr<SceneEntity>> sceneEntities;
};



#endif // SCENE_H
