#ifndef SCENE_H
#define SCENE_H

#include <glm/vec3.hpp>

#include <cstddef>	// std::size_t
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Interactions/PerspectiveCameraController.h"
#include "Rendering/RenderQueue.h"

struct EulerAngles;
class SceneEntity;



// Act as an Entity Manager, since there is a single Scene to take care of.
// Warning: shaders need to be all loaded before instantiating this class!
class Scene
{
public:
	Scene();

	// Virtual destructor (needed to handle any custom polymorphic deletion in child classes)
	virtual ~Scene();

	// Hash map of ptrs owning all Entities of the Scene.
	// Warning: ownership should never be transferred!
	std::unordered_map<RenderableType, std::vector<std::unique_ptr<SceneEntity>>> sceneEntities;

	// Count as "in-editor" camera rather than a Scene Entity that would be instantiated from the Application layer
	PerspectiveCameraController sceneViewer;

	virtual void Update(const float deltaTime);

	// Return a non-owning reference of a Scene Entity referred to by its ID
	template<typename EntityType = const SceneEntity>
	EntityType* GetEntity(const uint32_t entityID) const;

	// Return a non-owning reference of a Scene Entity referred to by its name
	template<typename EntityType = const SceneEntity>
	EntityType* GetEntity(const std::string& entityName) const;

protected:
	// Re-allocate the total number of bytes needed for the vector after adding the provided capacity
	void AllocateMemory(const size_t numOfBytes);

	// Make Scene Entity model matrix (i.e. transform) inherit the one from its parent provided as argument (called "attachment" or anchoring)
	void TagEntityAsAttached(const uint32_t entityIDBase, const uint32_t entityIDChild) const;

	uint32_t AddEntity(const RenderableType handle, std::unique_ptr<SceneEntity> inEntity);

	void SetSceneViewerTransformStart(const glm::vec3& inPosition, const EulerAngles& inRotation);
};



#endif // SCENE_H
