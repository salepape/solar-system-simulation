#include "CoreEngine.h"

#include <glfw/glfw3.h>
#include <glm/geometric.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <utility>

#include "Application/Application.h"
#include "Application/ApplicationControls.h"
#include "Application/Window.h"
#include "Cameras/Camera.h"
#include "Interactions/PerspectiveCameraController.h"
#include "Rendering/GlyphLoader.h"
#include "Rendering/Renderer.h"
#include "Rendering/ShaderLoader.h"
#include "Scene/SceneEntity.h"
#include "Scene/Transform.h"

CoreEngine* CoreEngine::instance = nullptr;



CoreEngine& CoreEngine::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CoreEngine();
	}

	return *instance;
}

void CoreEngine::SetUp()
{
	ShaderLibrary::BuildDefaultShaders();
	GlyphLibrary::LoadASCIICharacters();
}

void CoreEngine::PrepareSceneForRendering()
{
	if (scene != nullptr)
	{
		QueueRenderCommands();
	}
}

void CoreEngine::ClearSceneForRendering()
{
	if (scene != nullptr)
	{
		UnqueueRenderCommands();
	}
}

void CoreEngine::QueueRenderCommands()
{
	// Rendering layer - Apply by default on all IRenderables
	renderQueue.Push(
		RenderCommand{
			RenderableType::ALL,
			[]()
			{
				Renderer::EnableDepthTesting();
				Renderer::EnableBlending();

				// Enabling back face culling will discard inner-geometry rendering (i.e. celestial bodies)
				//Renderer::EnableBackFaceCulling();
			},
			[]()
			{
				// See comment above
				//Renderer::DisableBackFaceCulling();

				Renderer::DisableBlending();
				Renderer::DisableDepthTesting();
			}
		}
	);

	// Rendering layer 2 - Opaque background
	renderQueue.Push(
		RenderCommand{
			RenderableType::BACKGROUND,
			[&]()
			{
				scene->sceneViewer.GetCamera().SetProjectionViewVUniform(ViewMode::InfiniteLookAt, Application::GetInstance().GetWindow().GetAspectRatio());
			}
		}
	);

	// Rendering layer 3 - Opaque IRenderables
	renderQueue.Push(
		RenderCommand{
			RenderableType::OPAQUE_ENTITY,
			[&]()
			{
				scene->sceneViewer.GetCamera().SetProjectionViewVUniform(ViewMode::FiniteLookAt, Application::GetInstance().GetWindow().GetAspectRatio());
				scene->sceneViewer.GetCamera().SetPositionFUniform();
			}
		}
	);

	// Rendering layer 4 - Non-opaque IRenderables
	renderQueue.Push(
		RenderCommand{
			RenderableType::TRANSPARENT_ENTITY,
			[&]()
			{
				SortSceneEntitiesPerProximity(scene->sceneViewer.GetCamera().GetPosition());
			}
		}
	);
}

void CoreEngine::UnqueueRenderCommands()
{
	for (const RenderCommand& renderCommand : renderQueue.commands)
	{
		renderCommand.ClearDraw();
	}

	renderQueue.PopAll();
}

void CoreEngine::SortSceneEntitiesPerProximity(const glm::vec3& cameraPosition)
{
	// Custom std::vector ordering comparator so we draw from the farthest body first to the closest one last
	std::sort(scene->sceneEntities[RenderableType::TRANSPARENT_ENTITY].begin(), scene->sceneEntities[RenderableType::TRANSPARENT_ENTITY].end(),
		[&cameraPosition](const std::unique_ptr<SceneEntity>& e1, const std::unique_ptr<SceneEntity>& e2)
		{
			const ITransformable* const transformable1 = dynamic_cast<ITransformable*>(e1.get());
			const ITransformable* const transformable2 = dynamic_cast<ITransformable*>(e2.get());
			if (transformable1 == nullptr || transformable2 == nullptr)
			{
				std::cout << "ERROR::CORE_ENGINE - Attempt to get transform from a Scene Entity that does not implement ITransformable!" << std::endl;
				assert(false);
			}

			// @todo - Not very accurate, as the distance measure:
			// - takes into account the inner portion of body/orbit meshes
			// - does not compute the closest point of a surrounding geometry (in the case of orbits)
			return glm::distance(cameraPosition, transformable1->GetTransform().GetPosition()) > glm::distance(cameraPosition, transformable2->GetTransform().GetPosition());
		}
	);
}

void CoreEngine::Render()
{
	scene->sceneViewer.ProcessUserInput(timeBetweenFrames);

	for (const RenderCommand& renderCommand : renderQueue.commands)
	{
		renderCommand.PrepareDrawPerFrame();

		if (scene->sceneEntities.find(renderCommand.renderType) == scene->sceneEntities.end())
		{
			//std::cout << "ERROR::CORE_ENGINE - Handle for Scene Entity update has not been found!" << std::endl;
			//assert(false);

			// Only used by Renderable::ALL as falloff case since no Scene Entity is tagged as such

			continue;
		}

		for (const std::unique_ptr<SceneEntity>& sceneEntity : scene->sceneEntities[renderCommand.renderType])
		{
			// If current Scene Entity implements ITransformable interface (i.e. can be moved)
			if (ITransformable* const transformable = dynamic_cast<ITransformable*>(sceneEntity.get());
				transformable != nullptr)
			{
				transformable->ComputeTransformVUniform(
					timeBetweenFrames,
					scene->sceneViewer.GetCamera(),
					*scene->GetEntity<const ITransformable>(sceneEntity->parentID)
				);
			}

			// If current Scene Entity implements IRenderable interface (i.e. can be drawn)
			if (IRenderable* const renderable = dynamic_cast<IRenderable*>(sceneEntity.get());
				renderable != nullptr)
			{
				renderable->Render();
			}
		}
	}
}

void CoreEngine::Tick(const bool isPaused)
{
	// Delta time should always be computed, regardless of pause state
	if (isPaused == false)
	{
		elapsedPlayTime = GetElapsedTime() - elapsedPauseTime;
		timeBetweenFrames = elapsedPlayTime - lastFrameElapsedPlayTime;
		lastFrameElapsedPlayTime = elapsedPlayTime;
	}
	else
	{
		elapsedPauseTime = GetElapsedTime() - elapsedPlayTime;
		timeBetweenFrames = elapsedPauseTime - lastFrameElapsedPauseTime;
		lastFrameElapsedPauseTime = elapsedPauseTime;
	}

	Refresh();
}

void CoreEngine::Refresh()
{
	Renderer::ClearBufferTargets();

	ApplicationControls::ProcessUserInput();

	if (scene != nullptr)
	{
		scene->Update(timeBetweenFrames);
		Render();
	}
}

float CoreEngine::GetElapsedTime() const
{
	// @todo - Can we avoid static_casting at every frame?
	return static_cast<float>(glfwGetTime());
}

void CoreEngine::SetScene(std::unique_ptr<Scene> inScene)
{
	scene = std::move(inScene);
}
