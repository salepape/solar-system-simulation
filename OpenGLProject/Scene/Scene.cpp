#include "Scene.h"

#include <utility>

#include "Entities/SceneEntity.h"
#include "Rendering/Renderer.h"



Scene::Scene()
{
	Renderer::EnableDepthTesting();
	Renderer::EnableBlending();
	Renderer::EnableBackFaceCulling();
}

Scene::~Scene()
{
	Renderer::DisableBackFaceCulling();
	Renderer::DisableBlending();
	Renderer::DisableDepthTesting();
}

void Scene::Update(const float /*deltaTime*/)
{
	for (const std::unique_ptr<SceneEntity>& sceneEntity : sceneEntities)
	{
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

void Scene::AddEntity(std::unique_ptr<SceneEntity> inEntity)
{
	sceneEntities.push_back(std::move(inEntity));
}
