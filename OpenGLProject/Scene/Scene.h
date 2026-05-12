#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <cstddef>	// std::size_t
#include <vector>

class SceneEntity;



// @todo - Do SPIKE for going full ECS instead of only Entity-Component Composition relationships
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


	// @todo - Replace it by map sorted by handle keys. Handle = bits concatenation determining draw order (distance-based)
	// List of ptrs owning lifetime & memory management of Scene Entity instances.
	// Warning: ownership should not be moved away from this vector.
	std::vector<std::unique_ptr<SceneEntity>> sceneEntities;
};



#endif // SCENE_H
