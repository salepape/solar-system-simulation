#include <iostream>
#include <glad.h>
#include <glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

#include "Belt.h"
#include "Camera.h"
#include "Data.h"
#include "Model.h"
#include "Orbit.h"
#include "Renderer.h"
#include "Shader.h"
#include "Skybox.h"
#include "Sphere.h"
#include "Text.h"
#include "UniformBuffer.h"
#include "Window.h"





int main()
{
	constexpr unsigned int SCR_WIDTH = 1000;
	constexpr unsigned int SCR_HEIGHT = 1000;
	const float aspectRatio = static_cast<float>(SCR_WIDTH / SCR_HEIGHT);

	Window window(SCR_WIDTH, SCR_HEIGHT, "Solar System Simulation");

	Camera camera(glm::vec3(0.0f, 50.0f, 200.0f));
	window.camera = &camera;

	// Load all OpenGL function pointers locations using GLAD
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
	{
		std::cout << "ERROR::GLAD - Failed to initialise GLAD" << std::endl;
		return -1;
	}





	// Build and compile shader programs
	Shader defaultShader("DefaultShader.vs", "DefaultShader.fs");
	Shader textShader("TextShader.vs", "TextShader.fs");
	Shader skyboxShader("SkyboxShader.vs", "SkyboxShader.fs");
	Shader instancedModelShader("InstancedModelShader.vs", "InstancedModelShader.fs");

	// Create Milky Way skybox
	Skybox skybox("../Textures/MilkyWay/stars.dds");

	// Load the first 128 ASCII characters
	Text text(128);

	// Load models (meshes with textures applied)
	Model saturnRings("../Models/SaturnRings.obj");
	Model asteroid("../Models/Asteroid.obj");
	Model ice("../Models/Ice.obj");
	//Model deimos("../Models/Deimos.obj");
	//Model phobos("../Models/Phobos.obj");





	// Initialise all celestial bodies and their respective orbit (applying celectial body textures to have some colour consistency)
	LoadData();
	for (auto& dataInput: data)
	{
		if (dataInput.first == "Sun")
		{
			dataInput.second.sphere = new Sphere(dataInput.second.texturePath, dataInput.second.radius * 0.5f);
		}
		//else if (dataInput.first == "Deimos" || dataInput.first == "Phobos")
		//{
		//	// @todo - Have a reference of the non-spherical satellite (of type Model) in the data structure instead?
		//	dataInput.second.orbit = new Orbit(dataInput.second.texturePath, dataInput.second.dist);
		//}
		else
		{
			dataInput.second.sphere = new Sphere(dataInput.second.texturePath, dataInput.second.radius);
			dataInput.second.orbit = new Orbit(dataInput.second.texturePath, dataInput.second.dist);
		}
	}
	LoadPreComputations();

	// Do some instancing to build the main Solar Systems rock belts
	Belt asteroidBelt	{ {asteroid, 5000,  0.05f, 10},	{data["Mars"].dist * 1.1f,	  2.75f * DIST_SCALE_FACTOR * 1.0f / 2.5f,	0.4f} };
	Belt kuiperBelt		{ {ice,		 20000, 0.05f, 20},	{data["Neptune"].dist * 1.4f, 30.05f * DIST_SCALE_FACTOR,				0.4f} };





	// Compute the data size (in bytes) of each GLSL uniform variable, and their respective offset following the std140 layout base alignement rules 
	constexpr size_t GLSL_SCALAR_SIZE = 4;
	const size_t vec4Size = 4 * GLSL_SCALAR_SIZE;
	const size_t mat4v4Size = 16 * GLSL_SCALAR_SIZE;

	unsigned int uboBlockBindingPoint = 0;

	std::vector<unsigned int> matricesShadersIDs({ defaultShader.GetRendererID(), instancedModelShader.GetRendererID(), skyboxShader.GetRendererID(), textShader.GetRendererID() });
	UniformBuffer uboMatrices(matricesShadersIDs, "matrices", uboBlockBindingPoint++, static_cast <size_t>(2 * mat4v4Size));

	std::vector<unsigned int> entitiesShadersIDs({ defaultShader.GetRendererID(), instancedModelShader.GetRendererID() });

	struct Material
	{
		const glm::vec3 specular{ 0.0f, 0.0f, 0.0f };
		const float shininess{ 64.0f };
	} materialInstance;

	UniformBuffer uboMaterials(entitiesShadersIDs, "materialParameters", uboBlockBindingPoint++, static_cast<size_t>(mat4v4Size + GLSL_SCALAR_SIZE));
	uboMaterials.InitSubData({
		{ static_cast<const void*>(glm::value_ptr(materialInstance.specular)), vec4Size },
		{ static_cast<const void*>(&materialInstance.shininess), GLSL_SCALAR_SIZE }
		});

	struct Light
	{
		const glm::vec3 position{ 0.0f, 0.0f, 0.0f };

		// Terms of Phong shading formula 
		const glm::vec3 ambiant{ 0.2f, 0.2f, 0.2f };
		const glm::vec3 diffuse{ 0.95f, 0.95f, 0.95f };
		const glm::vec3 specular{ 1.0f, 1.0f, 1.0f };

		// Terms of attenuation spotlight formula
		const float constant{ 1.0f };
		const float linear{ 0.0007f };
		const float quadratic{ 0.000002f };

		const bool isBlinn{ false };
	} lightInstance;

	UniformBuffer uboLights(entitiesShadersIDs, "lightParameters", uboBlockBindingPoint++, static_cast<size_t>(4 * vec4Size + 4 * GLSL_SCALAR_SIZE));
	uboLights.InitSubData({
		{ static_cast<const void*>(glm::value_ptr(lightInstance.position)), vec4Size },
		{ static_cast<const void*>(glm::value_ptr(lightInstance.ambiant)), vec4Size },
		{ static_cast<const void*>(glm::value_ptr(lightInstance.diffuse)), vec4Size },
		{ static_cast<const void*>(glm::value_ptr(lightInstance.specular)), vec4Size },
		{ static_cast<const void*>(&lightInstance.constant), GLSL_SCALAR_SIZE },
		{ static_cast<const void*>(&lightInstance.linear), GLSL_SCALAR_SIZE },
		{ static_cast<const void*>(&lightInstance.quadratic), GLSL_SCALAR_SIZE },
		{ static_cast<const void*>(&lightInstance.isBlinn), GLSL_SCALAR_SIZE }
		});





	const float halfPi = 0.5f * glm::pi<float>();
	const glm::vec3 backVector(0.0f, 0.0f, 1.0f);
	const glm::vec3 rightVector(1.0f, 0.0f, 0.0f);
	const glm::vec3 upVector(0.0f, 1.0f, 0.0f);
	const glm::vec3 whiteColour(1.0f, 1.0f, 1.0f);





	// Create renderer
	Renderer renderer;
	renderer.DepthTest();

	// RENDER LOOP (running every frame)
	while (window.DoNotClose())
	{
		window.UpdateFrames();
		window.ProcessInput(camera);

		renderer.Clear();
		renderer.Blend();

		const glm::vec3& cameraPosition = camera.GetPosition();

		// Simulate a zoom - set far plane variable to a sufficiently high value 
		const glm::mat4& projectionMatrix = glm::perspective(glm::radians(camera.GetZoom()), aspectRatio, 0.1f, 1000.0f);

		// Simulate a camera circling around the scene 
		const glm::mat4& viewMatrix = camera.GetViewMatrix();

		uboMatrices.InitSubData({
			{ static_cast<const void*>(glm::value_ptr(projectionMatrix)), mat4v4Size },
			{ static_cast<const void*>(glm::value_ptr(viewMatrix)), mat4v4Size }
			});

		// Texture sampler ID (one for each object) 
		unsigned int samplerID = 0;

		defaultShader.Enable();
		defaultShader.setUniformVec3("viewPos", cameraPosition);

		// Draw celestial bodies, their orbits and their motion
		for (auto& dataInput: data)
		{
			// Both angles below are in radians
			float angleRot = 0.0f;
			float angleRotItself = 0.0f;

			if (dataInput.first != "Sun")
			{
				const float windowParams = window.GetCurrentFrame() * window.GetSimuSpeedFactor();

				// Angle of rotation around the sun (resp. planet) for planets (resp. moons) per frame
				angleRot = preComputations[dataInput.first].angleRotCst * windowParams;
				if (dataInput.second.parentInfo == nullptr)
				{
					dataInput.second.angleRot = angleRot;
				}

				// Angle of rotation of the celestial body around itself per frame
				angleRotItself = preComputations[dataInput.first].rotPeriodCst * windowParams;
			
				defaultShader.Enable();
				defaultShader.setUniformBool("isSun", false);
			}
			else
			{
				defaultShader.Enable();
				defaultShader.setUniformBool("isSun", true);
			}

			// Simulate movements that affects the current celestial bodies
			glm::mat4 defaultModelMatrix(1.0f);
			glm::mat4 orbitModelMatrix(1.0f);
			glm::mat4 textModelMatrix(1.0f);

			// Circular translation of satellite around corresponding planet
			if (const auto& planet = dataInput.second.parentInfo)
			{
				const float sinPlanetAngleRot = sin(planet->angleRot);
				
				defaultModelMatrix = glm::translate(defaultModelMatrix, glm::vec3(
					preComputations[dataInput.second.parentName].cosCircularTslCst * sinPlanetAngleRot,
					preComputations[dataInput.second.parentName].sinCircularTslCst * sinPlanetAngleRot,
					planet->dist * cos(planet->angleRot)));
				orbitModelMatrix = defaultModelMatrix;
			}

			// Orbital tilt (around axis colinear to orbit direction) + Circular translation along the orbit (equidistance to axis normal to orbital plane)
			const float sinAngleRot = sin(angleRot);
			defaultModelMatrix = glm::translate(defaultModelMatrix, glm::vec3(
				preComputations[dataInput.first].cosCircularTslCst * sinAngleRot,
				preComputations[dataInput.first].sinCircularTslCst * sinAngleRot,
				dataInput.second.dist * cos(angleRot)));
			textModelMatrix = defaultModelMatrix;

			// Axis tilt (around axis colinear to orbit direction)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, preComputations[dataInput.first].obliquityInRad, rightVector);

			// Rotation on itself (around axis normal to orbital plane)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, angleRotItself, upVector);

			// Draw Saturn rings
			if (dataInput.first == "Saturn")
			{
				defaultShader.Enable();
				defaultShader.setUniformBool("isSun", false);
				defaultShader.setUniformMat4("model", defaultModelMatrix);
				defaultShader.setUniformInt("materialDiffuse", samplerID);
				saturnRings.Render(renderer, samplerID++);
			}

			// Rotation on itself (to have celestial body poles vertical)
			defaultModelMatrix = glm::rotate(defaultModelMatrix, halfPi, rightVector);

			defaultShader.Enable();
			defaultShader.setUniformInt("materialDiffuse", samplerID);

			if (const auto& sphere = dataInput.second.sphere)
			{
				defaultShader.Enable();
				defaultShader.setUniformMat4("model", defaultModelMatrix);

				sphere->Render(renderer, samplerID++);
			}
			//// Non-spherical celestial bodies
			//else
			//{
			//	defaultModelMatrix = glm::scale(defaultModelMatrix, preComputations[dataInput.first].nonSphericalScaling);

			//	defaultShader.Enable();
			//	defaultShader.setUniformMat4("model", defaultModelMatrix);

			//	if (dataInput.first == "Deimos")
			//	{
			//		deimos.Render(renderer, samplerID++);
			//	}
			//	else if (dataInput.first == "Phobos")
			//	{
			//		phobos.Render(renderer, samplerID++);
			//	}
			//}





			// Draw billboard (containing current celestial body name) on top of current celestial body mesh/model
			if (window.IsSimuPaused())
			{
				// Orient text to camera position
				const glm::vec3& look = glm::normalize(cameraPosition - glm::vec3(textModelMatrix[3]));
				const glm::vec3& right = glm::cross(camera.GetUp(), look);
				const glm::vec3& up2 = cross(look, right);
				textModelMatrix[0] = glm::vec4(right, 0);
				textModelMatrix[1] = glm::vec4(up2, 0);
				textModelMatrix[2] = glm::vec4(look, 0);

				textShader.Enable();
				textShader.setUniformMat4("model", textModelMatrix);
				textShader.setUniformInt("texSampler", samplerID);
				textShader.setUniformVec3("textColor", whiteColour);

				//if (dataInput.first == "Deimos" || dataInput.first == "Phobos")
				//{
				//	// @todo - Get the bounding box size of models from Mesh class?
				//	text.Render(renderer, dataInput.first, 0.0f, dataInput.second.planetInfo->radius * 0.5f, dataInput.second.planetInfo->radius * 0.002f, samplerID++);
				//}
				//else 
				if (dataInput.first != "Sun")
				{
					text.Render(renderer, dataInput.first, 
						0.0f, preComputations[dataInput.first].textHeight, preComputations[dataInput.first].textScale, samplerID++);
				}
				else
				{
					text.Render(renderer, dataInput.first, 
						0.0f, preComputations[dataInput.first].sunTextHeight, preComputations[dataInput.first].sunTextScale, samplerID++);
				}
			}





			// Draw planet orbits
			if (dataInput.first != "Sun")
			{
				orbitModelMatrix = glm::rotate(orbitModelMatrix, preComputations[dataInput.first].orbInclinationInRad, backVector);

				defaultShader.Enable();
				defaultShader.setUniformMat4("model", orbitModelMatrix);
				defaultShader.setUniformInt("materialDiffuse", samplerID);
				dataInput.second.orbit->Render(renderer, samplerID++);
			}
		}





		// Draw the 2 main belts composed
		instancedModelShader.Enable();
		instancedModelShader.setUniformVec3("viewPos", cameraPosition);
		instancedModelShader.setUniformInt("materialDiffuse", samplerID);
		asteroidBelt.Render(renderer, samplerID++);

		instancedModelShader.setUniformInt("materialDiffuse", samplerID);
		kuiperBelt.Render(renderer, samplerID++);





		// Draw Milky Way skybox
		renderer.DepthEqual();
		uboMatrices.InitSubData({ {glm::value_ptr(glm::mat4(glm::mat3(viewMatrix))), mat4v4Size} }, mat4v4Size);
		skyboxShader.Enable();
		skyboxShader.setUniformInt("texSampler", samplerID);
		skybox.Render(renderer, samplerID++);
		renderer.DepthLess();





		window.SwapBuffers();
		window.PollEvents();
	}

	window.FreeUpResources();

	return 0;
}