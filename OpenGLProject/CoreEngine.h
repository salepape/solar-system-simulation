#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <glm/vec3.hpp>

#include <memory>

#include "Rendering/RenderQueue.h"
#include "Scene/Scene.h"

class Scene;



// @todo - Do SPIKE for going full ECS instead of only Entity-Component Composition relationships
class CoreEngine
{
public:
	// Unique Singleton instance defined in source file
	static CoreEngine& GetInstance();

	void SetUp();

	void PrepareSceneForRendering();
	void ClearSceneForRendering();

	void QueueRenderCommands();
	void UnqueueRenderCommands();

	// Main Render Loop (run every frame)
	void Tick(const bool isPaused);
	void Refresh();

	RenderQueue& GetRenderQueue() { return renderQueue; }

	void SetScene(std::unique_ptr<Scene> inScene);

	// Get time duration [in seconds] since the GLFW Window associated to the application has been created
	float GetElapsedTime() const;

private:
	static CoreEngine* instance;

	// Duration [in seconds] between the last frame and the current one (used to reduce processing power differences between computers)
	float timeBetweenFrames{ 0.0f };

	RenderQueue renderQueue;

	// Engine is focussed on rendering a single simulation scene for now
	std::unique_ptr<Scene> scene;

	// Time [in seconds] elapsed in Play mode since the GLFW Window associated to the application has been created
	float elapsedPlayTime{ 0.0f };
	// Time [in seconds] elapsed in Pause mode since the GLFW Window associated to the application has been created
	float elapsedPauseTime{ 0.0f };

	// Time cache [in seconds] to be able to compute Play delta time at next iteration
	float lastFrameElapsedPlayTime{ 0.0f };
	// Time cache [in seconds] to be able to compute Pause delta time at next iteration
	float lastFrameElapsedPauseTime{ 0.0f };

	// Render background, opaque and transparent Scene Entities (in this order) by setting up several passes according to previously defined render commands
	void Render();

	// @todo - To be done only for entities in FOV?
	// Set up rendering priority order of Scene Entities from farthest to closest according to camera.
	// Warning: to be used only on Scene Entities with an alpha in ]0.0f, 1.0f[, to render them per layers
	void SortSceneEntitiesPerProximity(const glm::vec3& cameraPosition);
};



#endif // CORE_ENGINE_H
