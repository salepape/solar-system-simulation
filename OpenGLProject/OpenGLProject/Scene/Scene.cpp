#include "Scene.h"

#include "Rendering/Renderer.h"



Scene::Scene()
{
	Renderer::EnableDepthTesting();
	Renderer::EnableBlending();
	// @todo - Optimise rendering by activating face culling only when the controller is outside spheres
	//Renderer::EnableFaceCulling();
}

void Scene::Clear() const
{
	//Renderer::DisableFaceCulling();
	Renderer::DisableBlending();
	Renderer::DisableDepthTesting();
}
