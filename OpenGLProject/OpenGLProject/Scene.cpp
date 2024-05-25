#include "Scene.h"

#include "Application.h"
#include "Window.h"



Scene::Scene() :
	runningApp(Application::GetInstance()), openedWindow(runningApp.GetWindow()), textRenderer(renderer)
{
	renderer.EnableDepthTesting();
	renderer.EnableBlending();
	// @todo - Optimise rendering by activating face culling only when the controller is outside spheres
	//renderer.EnableFaceCulling();
}

void Scene::Clear()
{
	//renderer.DisableFaceCulling();
	renderer.DisableBlending();
	renderer.DisableDepthTesting();
}