#include "Scene.h"

#include <glm/geometric.hpp>

#include <unordered_map>
#include <utility>

#include "Application/Application.h"
#include "Application/Window.h"
#include "Cameras/Camera.h"
#include "CoreEngine.h"
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
	SceneEntity* const eChild = GetEntity(entityIDChild);
	eChild->parentID = entityIDBase;
}

uint32_t Scene::AddEntity(const RenderType handle, std::unique_ptr<SceneEntity> inEntity)
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
		for (const std::unique_ptr<SceneEntity>& entityPtr : entityPair.second)
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
		for (const std::unique_ptr<SceneEntity>& entityPtr : entityPair.second)
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
	return GetEntity(entityID);
}

const SceneEntity* Scene::GetConstEntity(const std::string& entityName) const
{
	return GetEntity(entityName);
}

void Scene::SetSceneViewerTransformStart(const glm::vec3& inPosition, const EulerAngles& inRotationInRad)
{
	sceneViewer.GetCamera().SetInitialTransform(inPosition, inRotationInRad);
}
