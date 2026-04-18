#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <glm/mat4x4.hpp>

#include <cstdint>
#include <string>



// Represent a 'Game Object', i.e. a name and a Transform for now
class SceneEntity
{
public:
	// Default constructor (not needed)
	SceneEntity() = delete;

	// User-defined constructor (needed to be defined explictly in the constructor of each child class)
	SceneEntity(const std::string& inName);

	// Copy constructor (needed when a copy constructor of any child class is called)
	SceneEntity(const SceneEntity& inSceneEntity) = default;
	SceneEntity& operator = (const SceneEntity& inSceneEntity) = delete;

	// Move constructor (needed when a move constructor of any child class is called)
	SceneEntity(SceneEntity&& inSceneEntity) = default;
	SceneEntity&& operator = (SceneEntity&& inSceneEntity) = delete;

	// Virtual destructor (needed, as class is not final)
	virtual ~SceneEntity() = default;

	[[maybe_unused]] int32_t GetID() const { return ID; }
	const std::string& GetName() const { return name; }

	const glm::mat4& GetModelMatrix() const { return modelMatrix; }

	// @todo - Empty body in several child classes, and implementation with different params. Should be moved to interface?
	virtual void Render(const float elapsedTime = 0.0f) = 0;

protected:
	int32_t ID{ -1 };
	std::string name;

	// Correspond to the Scene Transform (position, rotation, scale) of the Entity
	glm::mat4 modelMatrix{ 1.0f };

	// @todo - Empty body in several child classes, and implementation with different params. Should be moved to interface?
	virtual void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) = 0;

private:
	static uint32_t entityIDCounter;
};



#endif // SCENE_ENTITY_H