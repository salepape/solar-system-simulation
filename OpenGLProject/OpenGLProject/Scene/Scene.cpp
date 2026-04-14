#include "Scene.h"

#include "OpenGLProject/Application/Application.h"
#include "OpenGLProject/Application/Window.h"



Scene::Scene() :
	runningApp(Application::GetInstance()), openWindow(runningApp.GetWindow())
{
	renderer.EnableDepthTesting();
	renderer.EnableBlending();
	// @todo - Optimise rendering by activating face culling only when the controller is outside spheres
	//renderer.EnableFaceCulling();
}

void Scene::Clear() const
{
	//renderer.DisableFaceCulling();
	renderer.DisableBlending();
	renderer.DisableDepthTesting();
}