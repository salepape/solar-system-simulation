#include "Scene.h"

#include <glm/geometric.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>

#include "Application/Application.h"
#include "Application/Window.h"
#include "Entities/SceneEntity.h"
#include "Rendering/Renderer.h"



Scene::Scene() :
	sceneViewer(glm::vec3(0.0f), glm::vec3(0.0f), 45.0f, 1.0e8f)
{
	// @todo - Flaw in engine code where we need to cache controller in window (until event system implemented)
	Window& currentWindow = Application::GetInstance().GetWindow();
	currentWindow.cameraController = &sceneViewer;

	Renderer::EnableDepthTesting();
	Renderer::EnableBlending();
	//Renderer::EnableBackFaceCulling();
}

Scene::~Scene()
{
	//Renderer::DisableBackFaceCulling();
	Renderer::DisableBlending();
	Renderer::DisableDepthTesting();
}

void Scene::Update(const float deltaTime, const std::optional<SceneEntityHandle> handle)
{
	// Handle: packed ID composed of several bytes of info giving priority of which to render first (distance-based)
	// Render command: list of OpenGL bindings ("to submit to GPU") to activate alongside handle 
	// Render queue: go through map, where each packed ID corresponds to batch of IRenderables which needs same Shader/UBO bindings

	if (handle.has_value() == false || sceneEntities.find(handle.value()) == sceneEntities.end())
	{
		std::cout << "SCENE::UPDATE - Handle for Scene Entity update has not been found!" << std::endl;
		assert(false);
	}

	for (const std::unique_ptr<SceneEntity>& sceneEntity : sceneEntities[handle.value()])
	{
		// If current Scene Entity implements ITransformable interface
		if (ITransformable* const transformable = dynamic_cast<ITransformable*>(sceneEntity.get());
			transformable != nullptr)
		{
			// @todo - Fix code inelegance (due to management of reference or nullopt not supported by ternary operator
			if (const SceneEntity* parentEntity = Scene::GetConstEntity(sceneEntity->parentID);
				parentEntity != nullptr)
			{
				transformable->ComputeModelMatrixVUniform(
					deltaTime,
					sceneViewer.GetCamera(),
					*parentEntity
				);
			}
			else
			{
				transformable->ComputeModelMatrixVUniform(
					deltaTime,
					sceneViewer.GetCamera()
				);
			}
		}

		// If current Scene Entity implements IRenderable interface
		if (IRenderable* const renderable = dynamic_cast<IRenderable*>(sceneEntity.get());
			renderable != nullptr)
		{
			renderable->Render();
		}
	}
}

void Scene::AllocateMemory(const size_t numOfBytes)
{
	sceneEntities.reserve(sceneEntities.size() + numOfBytes);
}

void Scene::TagEntityAsAttached(const uint32_t entityIDBase, const uint32_t entityIDChild)
{
	SceneEntity* const eChild = GetEntity(entityIDChild);
	eChild->parentID = entityIDBase;
}

uint32_t Scene::AddEntity(const SceneEntityHandle handle, std::unique_ptr<SceneEntity> inEntity)
{
	const uint32_t addedEntityID = inEntity->GetID();
	sceneEntities[handle].push_back(std::move(inEntity));

	return addedEntityID;
}

SceneEntity* Scene::GetEntity(const uint32_t entityID) const
{
	if (entityID == 0)
	{
		return nullptr;
	}

	for (const auto& entityPair : sceneEntities)
	{
		for (const auto& entityPtr : entityPair.second)
		{
			if (entityPtr->GetID() == entityID)
			{
				return entityPtr.get();
			}
		}
	}

	return nullptr;
}

SceneEntity* Scene::GetEntity(const std::string& entityName) const
{
	if (entityName.length() == 0)
	{
		return nullptr;
	}

	for (const auto& entityPair : sceneEntities)
	{
		for (const auto& entityPtr : entityPair.second)
		{
			if (entityPtr->GetName() == entityName)
			{
				return entityPtr.get();
			}
		}
	}

	return nullptr;
}

const SceneEntity* Scene::GetConstEntity(const uint32_t entityID) const
{
	if (entityID == 0)
	{
		return nullptr;
	}

	for (const auto& entityPair : sceneEntities)
	{
		for (const auto& entityPtr : entityPair.second)
		{
			if (entityPtr->GetID() == entityID)
			{
				return entityPtr.get();
			}
		}
	}

	return nullptr;
}

const SceneEntity* Scene::GetConstEntity(const std::string& entityName) const
{
	if (entityName.length() == 0)
	{
		return nullptr;
	}

	for (const auto& entityPair : sceneEntities)
	{
		for (const auto& entityPtr : entityPair.second)
		{
			if (entityPtr->GetName() == entityName)
			{
				return entityPtr.get();
			}
		}
	}

	return nullptr;
}

void Scene::OrderForTransparencyPass(const glm::vec3& cameraPosition)
{
	// TO BE DONE ONLY FOR ENTITIES IN FOV I ASSUME?

	// Custom std::vector comparator so we order the closest body to the camera at the end
	std::sort(sceneEntities[SceneEntityHandle::TRANSPARENT_ENTITY].begin(), sceneEntities[SceneEntityHandle::TRANSPARENT_ENTITY].end(),
		[&cameraPosition](const std::unique_ptr<SceneEntity>& e1, const std::unique_ptr<SceneEntity>& e2)
		{
			//std::cout << "dist cam -> e1 pos = " << e1->GetModelMatrix()[3].x << std::endl;
			//std::cout << "dist cam -> e2 pos = " << e2->GetModelMatrix()[3].x << std::endl;

			return glm::distance(cameraPosition, e1->GetPosition()) < glm::distance(cameraPosition, e2->GetPosition());
		});
}

void Scene::SetSceneViewerTransformStart(const glm::vec3& inPosition, const glm::vec3& inRotation)
{
	sceneViewer.GetCamera().SetInitialTransform(inPosition, inRotation);
}