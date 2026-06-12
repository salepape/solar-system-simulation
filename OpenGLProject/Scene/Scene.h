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

class SceneEntity;



// @todo - Do SPIKE for going full ECS instead of only Entity-Component Composition relationships
// Contain entities that model the simulation. Be careful: shaders need to be all loaded first!
class Scene
{
public:
	Scene();

	// Virtual destructor (needed to handle any custom polymorphic deletion in child classes)
	virtual ~Scene();

	virtual void Update(const float deltaTime);

	void QueueRenderCommands();
	void UnqueueRenderCommands();

	// Re-allocate the total number of bytes needed for the vector after adding the provided capacity
	void AllocateMemory(const size_t numOfBytes);

	// Make Scene Entity model matrix (i.e. transform) inherit the one from its parent provided as argument (called "attachment" or anchoring)
	void TagEntityAsAttached(const uint32_t entityIDBase, const uint32_t entityIDChild) const;

protected:
	uint32_t AddEntity(const RenderType handle, std::unique_ptr<SceneEntity> inEntity);

	// Return a non-owning reference of a Scene Entity referred to by its ID
	SceneEntity* GetEntity(const uint32_t entityID) const;

	// Return a non-owning reference of a Scene Entity referred to by its name
	SceneEntity* GetEntity(const std::string& entityName) const;

	// Return a non-owning reference of a const Scene Entity referred to by its ID
	const SceneEntity* GetConstEntity(const uint32_t entityID) const;

	// Return a non-owning reference of a const Scene Entity referred to by its name
	const SceneEntity* GetConstEntity(const std::string& entityName) const;

	// @todo - To be done only for entities in FOV
	// Sort scene entities with level of transparency from farthest to closest according to camera, to render overlapping non-opaque objects correctly per frame
	void OrderForTransparencyPass(const glm::vec3& cameraPosition);

	void SetSceneViewerTransformStart(const glm::vec3& inPosition, const EulerAngles& inRotation);

	// Count as "in-editor" camera rather than a Scene Entity that would be instantiated from the Application layer
	PerspectiveCameraController sceneViewer;

	// @todo - Replace it by map sorted by handle keys. Handle = bits concatenation determining draw order (distance-based)
	// Hash map of ptrs owning all Entities of the Scene.
	// Warning: ownership should never be transferred!
	std::unordered_map<RenderType, std::vector<std::unique_ptr<SceneEntity>>> sceneEntities;
};



#endif // SCENE_H
