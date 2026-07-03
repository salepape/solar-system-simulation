#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <cstdint>
#include <functional>
#include <optional>
#include <string>

#include "Transform.h"



// Should be "implemented" by all Scene Entity child classes that can be drawable/renderable on screen
class IRenderable
{
public:
	// Constain OpenGL functiosns that draw meshes with materials applied on them to the screen in a certain order
	virtual void Render() = 0;
};

class ITransformable
{
public:
	virtual void ComputeTransformVUniform(const float deltaTime, const class Camera& camera, std::optional<std::reference_wrapper<const class SceneEntity>> parentEntity = std::nullopt) = 0;
	//virtual void Attach() = 0;
};



// Abstract representation of a 'Game Object', i.e. a name and a Transform for now
class SceneEntity
{
public:
	// Default constructor (not needed)
	SceneEntity() = delete;

	// User-defined constructor (needed to be defined explictly in the constructor of each child class)
	SceneEntity(const std::string& inName);

	// Copy constructor (not needed - SCENE ENTITY GETTER RETURN NON-OWNING RAW PTR, HENCE NOT NEEDED)
	SceneEntity(const SceneEntity& inSceneEntity) = delete;
	SceneEntity& operator = (const SceneEntity& inSceneEntity) = delete;

	// Move constructor (not needed - SCENE ENTITY GETTER RETURN NON-OWNING RAW PTR, HENCE NOT NEEDED)
	SceneEntity(SceneEntity&& inSceneEntity) = delete;
	SceneEntity&& operator = (SceneEntity&& inSceneEntity) = delete;

	// Virtual destructor (needed, as class is not final)
	virtual ~SceneEntity() = default;

	uint32_t GetID() const { return ID; }
	const std::string& GetName() const { return name; }
	const Transform& GetTransform() const { return transform; }

	uint32_t parentID{ 0 };

protected:
	uint32_t ID{ 0 };
	std::string name;

	// @todo - Not supposed to be there, if child classes implement ITransformable
	Transform transform;

private:
	static uint32_t entityIDCounter;
};



#endif // SCENE_ENTITY_H
