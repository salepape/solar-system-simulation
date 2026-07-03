#include "Scene.h"

#include <glm/geometric.hpp>

#include <iostream>
#include <unordered_map>
#include <utility>

#include "Application/Application.h"
#include "Application/Window.h"
#include "Cameras/Camera.h"
#include "CoreEngine.h"
#include "Entities/CelestialBodyEntity.h"
#include "SceneEntity.h"
#include "Transform.h"



Scene::Scene() :
	sceneViewer(glm::vec3(0.0f), EulerAngles{}, 45.0f, 1.0e8f)
{
	// @todo - Flaw in engine code where we need to cache controller in window (until event system implemented)
	Window& currentWindow = Application::GetInstance().GetWindow();
	currentWindow.cameraController = &sceneViewer;
}

Scene::~Scene()
{

}

void Scene::Update(const float deltaTime)
{
	sceneViewer.ProcessUserInput(deltaTime);
}

void Scene::AllocateMemory(const size_t numOfBytes)
{
	sceneEntities.reserve(sceneEntities.size() + numOfBytes);
}

void Scene::TagEntityAsAttached(const uint32_t entityIDBase, const uint32_t entityIDChild) const
{
	SceneEntity* const eChild = GetEntity<SceneEntity>(entityIDChild);
	eChild->parentID = entityIDBase;
}

uint32_t Scene::AddEntity(const RenderableType handle, std::unique_ptr<SceneEntity> inEntity)
{
	const uint32_t addedEntityID = inEntity->GetID();
	sceneEntities[handle].push_back(std::move(inEntity));

	return addedEntityID;
}

template<typename EntityType>
EntityType* Scene::GetEntity(const uint32_t entityID) const
{
	if (entityID == 0)
	{
		return nullptr;
	}

	for (const auto& entityPair : sceneEntities)
	{
		for (const std::unique_ptr<SceneEntity>& entityPtr : entityPair.second)
		{
			if (entityPtr->GetID() == entityID)
			{
				EntityType* const downcastedEntity = dynamic_cast<EntityType*>(entityPtr.get());
				if (downcastedEntity == nullptr)
				{
					std::cout << "ERROR::SCENE - Scene Entity has failed being downcasted" << std::endl;
					assert(false);
				}

				return downcastedEntity;
			}
		}
	}

	return nullptr;
}

// Specialisation prototypes needed here in the source file, so function definition above doesn't have to be included in the header file
template SceneEntity* Scene::GetEntity<SceneEntity>(const uint32_t entityID) const;
template const SceneEntity* Scene::GetEntity<const SceneEntity>(const uint32_t entityID) const;
template ITransformable* Scene::GetEntity<ITransformable>(const uint32_t entityID) const;
template const ITransformable* Scene::GetEntity<const ITransformable>(const uint32_t entityID) const;
template CelestialBodyEntity* Scene::GetEntity<CelestialBodyEntity>(const uint32_t entityID) const;
template const CelestialBodyEntity* Scene::GetEntity<const CelestialBodyEntity>(const uint32_t entityID) const;

template<typename EntityType>
EntityType* Scene::GetEntity(const std::string& entityName) const
{
	if (entityName.length() == 0)
	{
		return nullptr;
	}

	for (const auto& entityPair : sceneEntities)
	{
		for (const std::unique_ptr<SceneEntity>& entityPtr : entityPair.second)
		{
			if (entityPtr->GetName() == entityName)
			{
				EntityType* const downcastedEntity = dynamic_cast<EntityType*>(entityPtr.get());
				if (downcastedEntity == nullptr)
				{
					std::cout << "ERROR::SCENE - Scene Entity has failed being downcasted" << std::endl;
					assert(false);
				}

				return downcastedEntity;
			}
		}
	}

	return nullptr;
}

// Specialisation prototypes needed here in the source file, so function definition above doesn't have to be included in the header file
template SceneEntity* Scene::GetEntity<SceneEntity>(const std::string& entityName) const;
template const SceneEntity* Scene::GetEntity<const SceneEntity>(const std::string& entityName) const;
template ITransformable* Scene::GetEntity<ITransformable>(const std::string& entityName) const;
template const ITransformable* Scene::GetEntity<const ITransformable>(const std::string& entityName) const;
template CelestialBodyEntity* Scene::GetEntity<CelestialBodyEntity>(const std::string& entityName) const;
template const CelestialBodyEntity* Scene::GetEntity<const CelestialBodyEntity>(const std::string& entityName) const;

void Scene::SetSceneViewerTransformStart(const glm::vec3& inPosition, const EulerAngles& inRotationInRad)
{
	sceneViewer.GetCamera().SetInitialTransform(inPosition, inRotationInRad);
}
