#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <cstdint>
//#include <filesystem>
#include <functional>
#include <optional>
#include <string>

//#include "Rendering/BlinnPhongMaterial.h"



// Should be "implemented" by all Scene Entity child classes that can be drawable/renderable on screen
class IRenderable
{
public:
	//virtual BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& texturePath) = 0;
	virtual void Render() = 0;
};

class ITransformable
{
public:
	virtual void ComputeModelMatrixVUniform(const float deltaTime, const class Camera& camera, std::optional<std::reference_wrapper<const class SceneEntity>> parentEntity = std::nullopt) = 0;
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

	// Copy constructor (needed when a copy constructor of any child class is called)
	SceneEntity(const SceneEntity& inSceneEntity) = default;
	SceneEntity& operator = (const SceneEntity& inSceneEntity) = delete;

	// Move constructor (needed when a move constructor of any child class is called)
	SceneEntity(SceneEntity&& inSceneEntity) = default;
	SceneEntity&& operator = (SceneEntity&& inSceneEntity) = delete;

	// Virtual destructor (needed, as class is not final)
	virtual ~SceneEntity() = default;

	uint32_t GetID() const { return ID; }
	const std::string& GetName() const { return name; }

	const glm::mat4& GetModelMatrix() const { return modelMatrix; }
	void SetModelMatrix(const glm::mat4& inModelMatrix) { modelMatrix = inModelMatrix; }

	glm::vec3 GetPosition() const { return glm::vec3(modelMatrix[3]); }

	uint32_t parentID{ 0 };

protected:
	uint32_t ID{ 0 };
	std::string name;

	// Correspond to the Scene Transform (position, rotation, scale) of the Scene Entity
	glm::mat4 modelMatrix{ 1.0f };

private:
	static uint32_t entityIDCounter;
};



#endif // SCENE_ENTITY_H
