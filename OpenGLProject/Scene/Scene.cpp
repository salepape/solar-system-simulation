#include "Scene.h"

#include <glm/geometric.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>

#include "Application/Application.h"
#include "Application/Window.h"
#include "Rendering/Renderer.h"
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

	// Handle: packed ID composed of several bytes of info giving priority of which to render first (distance-based)
	// Render command: list of OpenGL bindings ("to submit to GPU") to activate alongside handle 
	// Render queue: go through map, where each packed ID corresponds to batch of IRenderables which needs same Shader/UBO bindings

	// RENDER QUEUE ORDER: 1) BACKGROUND 2) OPAQUE ENTITIES 3) NON-OPAQUE ENTITIES
	// RENDER COMMAND: CHAIN OF GL FUNCTIONS ASSOCIATED WITH GROUP OF ENTITIES TO RENDER

	for (const RenderCommand& renderCommand : Application::GetInstance().GetRenderQueue().queue)
	{
		renderCommand.Queue();

		if (sceneEntities.find(renderCommand.renderType) == sceneEntities.end())
		{
			/*std::cout << "SCENE::UPDATE - Handle for Scene Entity update has not been found!" << std::endl;
			assert(false);*/

			continue;
		}

		for (const std::unique_ptr<SceneEntity>& sceneEntity : sceneEntities[renderCommand.renderType])
		{
			// If current Scene Entity implements ITransformable interface
			if (ITransformable* const transformable = dynamic_cast<ITransformable*>(sceneEntity.get());
				transformable != nullptr)
			{
				// @todo - Fix code inelegance (due to management of reference or nullopt not supported by ternary operator
				if (const SceneEntity* parentEntity = Scene::GetConstEntity(sceneEntity->parentID);
					parentEntity != nullptr)
				{
					transformable->ComputeTransformVUniform(
						deltaTime,
						sceneViewer.GetCamera(),
						*parentEntity
					);
				}
				else
				{
					transformable->ComputeTransformVUniform(
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

void Scene::QueueRenderCommands()
{
	RenderQueue& renderQueue = Application::GetInstance().GetRenderQueue();

	renderQueue.Push(
		RenderCommand{
			RenderType::ALL,
			[]()
			{
				Renderer::EnableDepthTesting();
				Renderer::EnableBlending();
				//Renderer::EnableBackFaceCulling();
			},
			[]()
			{
			//Renderer::DisableBackFaceCulling();
			Renderer::DisableBlending();
			Renderer::DisableDepthTesting();
		}
		}
	);

	// MILKY WAY (not part of Scene Entities hash map, member variable of SS class)	
	renderQueue.Push(
		RenderCommand{
			RenderType::BACKGROUND,
			[&]()
			{
				sceneViewer.GetCamera().SetProjectionViewVUniform(ViewMode::InfiniteLookAt, Application::GetInstance().GetWindow().GetAspectRatio());
			}
		}
	);

	// CELESTIAL BODIES, ORBITS, BILLBOARDS & BELTS (part of Scene Entities hash map)
	renderQueue.Push(
		RenderCommand{
			RenderType::OPAQUE_ENTITY,
			[&]()
			{
				sceneViewer.GetCamera().SetProjectionViewVUniform(ViewMode::FiniteLookAt, Application::GetInstance().GetWindow().GetAspectRatio());
				sceneViewer.GetCamera().SetPositionFUniform();
			}
		}
	);

	// BODY RINGS (part of Scene Entities hash map)
	renderQueue.Push(
		RenderCommand{
			RenderType::TRANSPARENT_ENTITY,
			[&]()
			{
				Scene::OrderForTransparencyPass(sceneViewer.GetCamera().GetPosition());
			}
		}
	);
}

void Scene::UnqueueRenderCommands()
{
	RenderQueue& renderQueue = Application::GetInstance().GetRenderQueue();

	for (const RenderCommand& renderCommand : renderQueue.queue)
	{
		renderCommand.Unqueue();
	}

	renderQueue.PopAll();
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
	// Custom std::vector comparator so we order the closest body to the camera at the end
	std::sort(sceneEntities[RenderType::TRANSPARENT_ENTITY].begin(), sceneEntities[RenderType::TRANSPARENT_ENTITY].end(),
		[&cameraPosition](const std::unique_ptr<SceneEntity>& e1, const std::unique_ptr<SceneEntity>& e2)
		{
			return glm::distance(cameraPosition, e1->GetTransform().GetPosition()) < glm::distance(cameraPosition, e2->GetTransform().GetPosition());
		});
}

void Scene::SetSceneViewerTransformStart(const glm::vec3& inPosition, const EulerAngles& inRotationInRad)
{
	sceneViewer.GetCamera().SetInitialTransform(inPosition, inRotationInRad);
}
