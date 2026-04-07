#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <string>

#include "BlinnPhongMaterial.h"

class Renderer;



// Represent a 'Game Object', i.e. a name, a Transform, and a Material instance mainly
class SceneEntity
{
public:
	// Default constructor (not needed)
	SceneEntity() = delete;

	// User-defined constructor (needed to be defined explictly in the constructor of each child class)
	SceneEntity(std::string inName, BlinnPhongMaterial inMaterial);

	// Copy constructor (needed when a copy constructor of any child class is called)
	SceneEntity(const SceneEntity& inSceneEntity) = default;
	SceneEntity& operator = (const SceneEntity& inSceneEntity) = delete;

	// Move constructor (needed when a move constructor of any child class is called)
	SceneEntity(SceneEntity&& inSceneEntity) = default;
	SceneEntity&& operator = (SceneEntity&& inSceneEntity) = delete;

	// Virtual destructor (needed, as class is not final)
	virtual ~SceneEntity() = default;

	int32_t GetID() const { return ID; }
	const std::string& GetName() const { return name; }

	const BlinnPhongMaterial& GetMaterial() const { return material; }
	const glm::mat4& GetModelMatrix() const { return modelMatrix; }

	// @todo - Empty body in several child classes, and implementation with different params
	virtual void Render(const Renderer& renderer, const float elapsedTime = 0.0f) = 0;

protected:
	int32_t ID{ -1 };
	std::string name;

	// @todo - Not sure instantiating a Material here is the best place, as Model Material instancing is different
	// We assume only a single Material instance can be linked to a Scene Entity, which implies only a single Mesh exists for it
	BlinnPhongMaterial material;

	glm::mat4 modelMatrix{ 1.0f };

	// @todo - Empty body in several child classes, and implementation with different params
	virtual void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) = 0;

	// Called on a per-frame basis to update the camera view.
	// Shader should already be enabled in each Scene Entity child Render() method prior to call this one. Uniform to be updated in child classes
	void SetModelMatrixVUniform();

private:
	static uint32_t entityIDCounter;

	void SetVUniforms() const;
};



#endif // SCENE_ENTITY_H