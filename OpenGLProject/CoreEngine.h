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

	// Time [in seconds] between the last frame and the current one (used to reduce processing power differences between computers)
	float deltaTime{ 0.0f };

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

	void Render(const float deltaTime);

	// @todo - To be done only for entities in FOV
	// Sort scene entities with level of transparency from farthest to closest according to camera, to render overlapping non-opaque objects correctly per frame
	void OrderForTransparencyPass(const glm::vec3& cameraPosition);
};



#endif // CORE_ENGINE_H
