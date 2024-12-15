#include "Scene.h"

#include "Application.h"
#include "Window.h"



// @todo - Move the font path to the SolarSystem child class
Scene::Scene() :
	runningApp(Application::GetInstance()), openWindow(runningApp.GetWindow()), textRenderer(renderer, "../Fonts/arial.ttf")
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