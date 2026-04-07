#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <string>

#include "BlinnPhongMaterial.h"

class Renderer;



// Class that represents a 'Game Object'
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

	virtual void Render(const Renderer& renderer, const float elapsedTime = 0.0f) = 0;

protected:
	int32_t ID{ -1 };
	std::string name;

	BlinnPhongMaterial material;
	glm::mat4 modelMatrix{ 1.0f };

	virtual void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) = 0;

	// Called on a per-frame basis to update the camera view.
	// Shader should already be enabled in each Scene Entity child Render() method prior to call this one. Uniform to be updated in child classes
	void SetModelMatrixVUniform();

private:
	static uint32_t entityIDCounter;

	void SetVUniforms() const;
};



#endif // SCENE_ENTITY_H