#include "CoreEngine.h"

#include <glfw/glfw3.h>
#include <glm/geometric.hpp>

#include <algorithm>
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
	ShaderLoader::BuildShaders();
	GlyphLoader::LoadASCIICharacters();
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
				scene->sceneViewer.GetCamera().SetProjectionViewVUniform(ViewMode::InfiniteLookAt, Application::GetInstance().GetWindow().GetAspectRatio());
			}
		}
	);

	// CELESTIAL BODIES, ORBITS, BILLBOARDS & BELTS (part of Scene Entities hash map)
	renderQueue.Push(
		RenderCommand{
			RenderType::OPAQUE_ENTITY,
			[&]()
			{
				scene->sceneViewer.GetCamera().SetProjectionViewVUniform(ViewMode::FiniteLookAt, Application::GetInstance().GetWindow().GetAspectRatio());
				scene->sceneViewer.GetCamera().SetPositionFUniform();
			}
		}
	);

	// BODY RINGS (part of Scene Entities hash map)
	renderQueue.Push(
		RenderCommand{
			RenderType::TRANSPARENT_ENTITY,
			[&]()
			{
				OrderForTransparencyPass(scene->sceneViewer.GetCamera().GetPosition());
			}
		}
	);
}

void CoreEngine::UnqueueRenderCommands()
{
	for (const RenderCommand& renderCommand : renderQueue.queue)
	{
		renderCommand.Unqueue();
	}

	renderQueue.PopAll();
}

void CoreEngine::OrderForTransparencyPass(const glm::vec3& cameraPosition)
{
	// Custom std::vector comparator so we order the closest body to the camera at the end
	std::sort(scene->sceneEntities[RenderType::TRANSPARENT_ENTITY].begin(), scene->sceneEntities[RenderType::TRANSPARENT_ENTITY].end(),
		[&cameraPosition](const std::unique_ptr<SceneEntity>& e1, const std::unique_ptr<SceneEntity>& e2)
		{
			return glm::distance(cameraPosition, e1->GetTransform().GetPosition()) < glm::distance(cameraPosition, e2->GetTransform().GetPosition());
		});
}

void CoreEngine::Render(const float deltaTime)
{
	scene->sceneViewer.ProcessUserInput(deltaTime);

	// Handle: packed ID composed of several bytes of info giving priority of which to render first (distance-based)
	// Render command: list of OpenGL bindings ("to submit to GPU") to activate alongside handle 
	// Render queue: go through map, where each packed ID corresponds to batch of IRenderables which needs same Shader/UBO bindings

	// RENDER QUEUE ORDER: 1) BACKGROUND 2) OPAQUE ENTITIES 3) NON-OPAQUE ENTITIES
	// RENDER COMMAND: CHAIN OF GL FUNCTIONS ASSOCIATED WITH GROUP OF ENTITIES TO RENDER

	for (const RenderCommand& renderCommand : renderQueue.queue)
	{
		renderCommand.Queue();

		if (scene->sceneEntities.find(renderCommand.renderType) == scene->sceneEntities.end())
		{
			//std::cout << "SCENE::UPDATE - Handle for Scene Entity update has not been found!" << std::endl;
			//assert(false);

			continue;
		}

		for (const std::unique_ptr<SceneEntity>& sceneEntity : scene->sceneEntities[renderCommand.renderType])
		{
			// If current Scene Entity implements ITransformable interface
			if (ITransformable* const transformable = dynamic_cast<ITransformable*>(sceneEntity.get());
				transformable != nullptr)
			{
				// @todo - Fix code inelegance (due to management of reference or nullopt not supported by ternary operator
				if (const SceneEntity* parentEntity = scene->GetConstEntity(sceneEntity->parentID);
					parentEntity != nullptr)
				{
					transformable->ComputeTransformVUniform(
						deltaTime,
						scene->sceneViewer.GetCamera(),
						*parentEntity
					);
				}
				else
				{
					transformable->ComputeTransformVUniform(
						deltaTime,
						scene->sceneViewer.GetCamera()
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

void CoreEngine::Tick(const bool isPaused)
{
	// Delta time should always be computed, regardless of pause state
	if (isPaused == false)
	{
		elapsedPlayTime = GetElapsedTime() - elapsedPauseTime;
		deltaTime = elapsedPlayTime - lastFrameElapsedPlayTime;
		lastFrameElapsedPlayTime = elapsedPlayTime;
	}
	else
	{
		elapsedPauseTime = GetElapsedTime() - elapsedPlayTime;
		deltaTime = elapsedPauseTime - lastFrameElapsedPauseTime;
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
		scene->Update(deltaTime);
		Render(deltaTime);
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
