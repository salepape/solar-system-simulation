#include "ResourceLoader.h"

#include <algorithm>
#include <ctype.h>
#include <iostream>

#include "SolarSystem.h"
#include "Utils.h"



std::vector<Shader> shaders;
std::vector<UniformBuffer> ubos;

namespace ResourceLoader
{
	auto& celestialBodiesRef = SolarSystem::GetCelestialBodiesVector();
	auto& beltsRef = SolarSystem::GetBeltsVector();

	std::string GetNameFromTexturePath(const std::string& inTexturePath)
	{
		std::string bodyName;
		bodyName.reserve(inTexturePath.size());

		auto it = inTexturePath.begin();
		for (it; *it != '_'; ++it);

		++it;
		for (it; *it != '_' && *it != '.'; ++it)
		{
			bodyName.push_back(*it);
		}

		bodyName[0] = toupper(bodyName[0]);

		return bodyName;
	}

	void LoadShaders()
	{
		shaders.reserve(7);

		// RendererID will be identical for all shaders if we do not instantiate them line by line before pushing them into the vector
		static Shader celestialBodyShader("CelestialBody", "DefaultShader.vs", "DefaultShader.fs");
		static Shader sunShader("Sun", "DefaultShader.vs", "SunShader.fs");
		static Shader textShader("TextGlyph", "TextShader.vs", "TextShader.fs");
		static Shader beltBodyShader("BeltBody", "InstancedModelShader.vs", "DefaultShader.fs");
		static Shader skyboxShader("MilkyWay", "SkyboxShader.vs", "SkyboxShader.fs");
		static Shader saturnRingsShader("SaturnRings", "DefaultShader.vs", "DefaultShader.fs");
		static Shader orbitShader("Orbit", "DefaultShader.vs", "DefaultShader.fs");
		shaders.push_back(celestialBodyShader);
		shaders.push_back(sunShader);
		shaders.push_back(textShader);
		shaders.push_back(beltBodyShader);
		shaders.push_back(skyboxShader);
		shaders.push_back(saturnRingsShader);
		shaders.push_back(orbitShader);

		ubos.reserve(3);
		static UniformBuffer projectionViewUBO({ { GetShader("CelestialBody").GetRendererID(), GetShader("Sun").GetRendererID(), GetShader("TextGlyph").GetRendererID(), GetShader("BeltBody").GetRendererID(), GetShader("MilkyWay").GetRendererID(), GetShader("SaturnRings").GetRendererID(), GetShader("Orbit").GetRendererID() },
			"ubo_ProjectionView", Utils::mat4v4Size });
		static UniformBuffer cameraPositionUBO({ { GetShader("CelestialBody").GetRendererID(), GetShader("BeltBody").GetRendererID(), ResourceLoader::GetShader("Sun").GetRendererID(), ResourceLoader::GetShader("SaturnRings").GetRendererID(), ResourceLoader::GetShader("Orbit").GetRendererID() },
			"ubo_CameraPosition", Utils::vec4Size });
		static UniformBuffer pointLightParamsUBO({ { GetShader("CelestialBody").GetRendererID(), GetShader("BeltBody").GetRendererID(), GetShader("SaturnRings").GetRendererID(), GetShader("Orbit").GetRendererID() },
			"ubo_PointLightParams", 4 * Utils::vec4Size + 4 * Utils::scalarSize });
		ubos.push_back(projectionViewUBO);
		ubos.push_back(cameraPositionUBO);
		ubos.push_back(pointLightParamsUBO);
	}

	void LoadCelestialBodies()
	{
		celestialBodiesRef.reserve(BODIES_COUNT);

		/* Sun */		celestialBodiesRef.push_back({ texturePaths[0], 109.3f * 0.5f, 0.0f, 7.25f, 0.0f, 27.0f, 0.0f });
		/* Mercury */	celestialBodiesRef.push_back({ texturePaths[1],  0.383f, GetBody("Sun").radius * 2.0f + 0.38f * DIST_SCALE_FACTOR,			0.03f,  87.97f,	   58.6f,   7.01f });
		/* Venus */		celestialBodiesRef.push_back({ texturePaths[2],  0.95f,  GetBody("Mercury").distanceToParent + 0.72f * DIST_SCALE_FACTOR,	2.64f,  224.7f,   -243.02f, 3.39f });
		/* Earth */		celestialBodiesRef.push_back({ texturePaths[3],  1.0f,	GetBody("Venus").distanceToParent + 1.0f * DIST_SCALE_FACTOR,		23.44f, 365.26f,   0.99f,   0.0f });
		/* Mars */		celestialBodiesRef.push_back({ texturePaths[4],  0.532f, GetBody("Earth").distanceToParent + 1.52f * DIST_SCALE_FACTOR,		25.19f, 686.97f,   1.03f,   1.85f });
		/* Jupiter */	celestialBodiesRef.push_back({ texturePaths[5],  10.97f, GetBody("Mars").distanceToParent + 5.19f * DIST_SCALE_FACTOR,		3.13f,  4332.59f,  0.41f,   1.31f });
		/* Saturn */	celestialBodiesRef.push_back({ texturePaths[6],  9.14f,  GetBody("Jupiter").distanceToParent + 9.53f * DIST_SCALE_FACTOR,	26.73f, 10759.22f, 0.43f,   2.49f });
		/* Uranus */	celestialBodiesRef.push_back({ texturePaths[7],  3.981f, GetBody("Saturn").distanceToParent + 19.20f * DIST_SCALE_FACTOR,	82.23f, 30688.5,  -0.72f,   0.77f });
		/* Neptune */	celestialBodiesRef.push_back({ texturePaths[8],  3.865f, GetBody("Uranus").distanceToParent + 30.05f * DIST_SCALE_FACTOR,	28.32f, 60182.0f,  0.67f,   1.77f });

		/* Ceres */		celestialBodiesRef.push_back({ texturePaths[9],  0.074f, GetBody("Mars").distanceToParent + 2.75f * DIST_SCALE_FACTOR,		0.0f,   1683.15f,  9.07f,   17.14f });
		/* Pluto */		celestialBodiesRef.push_back({ texturePaths[10], 0.186f, GetBody("Neptune").distanceToParent + 39.24f * DIST_SCALE_FACTOR,	57.47f, 90560.0f, -6.39f,   10.62f });

		/* Luna */		celestialBodiesRef.push_back({ texturePaths[11], 0.273f, GetBody("Earth").radius + 0.384f * DIST_SCALE_FACTOR,				6.68f,  27.32f,    27.32f,  5.145f,	   GetBody("Earth").ID });
		/* Callisto */	celestialBodiesRef.push_back({ texturePaths[12], 0.378f, GetBody("Jupiter").radius + 1.883f * DIST_SCALE_FACTOR,			0.0f,   16.69f,    16.69f,  2.017f,	   GetBody("Jupiter").ID });
		/* Europa */	celestialBodiesRef.push_back({ texturePaths[13], 0.245f, GetBody("Jupiter").radius + 0.671f * DIST_SCALE_FACTOR,			0.1f,   3.55f,     3.55f,   1.791f,	   GetBody("Jupiter").ID });
		/* Ganymede */	celestialBodiesRef.push_back({ texturePaths[14], 0.413f, GetBody("Jupiter").radius + 1.07f * DIST_SCALE_FACTOR,				0.16f,  7.15f,     7.15f,   2.214f,    GetBody("Jupiter").ID });
		/* Io */		celestialBodiesRef.push_back({ texturePaths[15], 0.286f, GetBody("Jupiter").radius + 0.422f * DIST_SCALE_FACTOR,			0.0f,   1.77f,     1.77f,   2.213f,	   GetBody("Jupiter").ID });
		/* Titan */		celestialBodiesRef.push_back({ texturePaths[16], 0.404f, GetBody("Saturn").radius + 1.222f * DIST_SCALE_FACTOR,				0.0f,   15.95f,    15.95f,  0.0f,      GetBody("Saturn").ID });
		/* Triton */	celestialBodiesRef.push_back({ texturePaths[17], 0.212f, GetBody("Neptune").radius + 0.354f * DIST_SCALE_FACTOR,			0.0f,   5.88f,     5.88f,   129.812f,  GetBody("Neptune").ID });
		// /* Deimos */	celestialBodiesRef.push_back({ texturePaths[18], 0.000f, GetBody("Mars").radius + 0.156f * DIST_SCALE_FACTOR,				0.0f,   1.263f,    1.263f,	27.58f,	   GetBody("Mars").ID });
		// /* Phobos */	celestialBodiesRef.push_back({ texturePaths[19], 0.000f, GetBody("Mars").radius + 0.94f * DIST_SCALE_FACTOR,				0.0f,	0.319f,    0.319f,  28.4f,	   GetBody("Mars").ID });
	}

	void LoadBelts()
	{
		beltsRef.reserve(BELTS_COUNT);

		beltsRef.push_back({ { "../Models/Asteroid.obj",  5000,	0.05f,	10 }, { GetBody("Mars").distanceToParent * 1.1f,	2.75f * ResourceLoader::DIST_SCALE_FACTOR * 1.0f / 2.5f,	0.4f } });
		beltsRef.push_back({ { "../Models/Ice.obj",		 20000,	0.05f,	20 }, { GetBody("Neptune").distanceToParent * 1.4f,	30.05f * ResourceLoader::DIST_SCALE_FACTOR,					0.4f } });
	}

	CelestialBody& GetBody(const std::string& inBodyName)
	{
		const auto& bodyIt = std::find_if(celestialBodiesRef.begin(), celestialBodiesRef.end(), [&inBodyName](const CelestialBody& celestialBody)
		{
			return celestialBody.name == inBodyName;
		});

		if (bodyIt == celestialBodiesRef.end())
		{
			std::cout << "ERROR::RESOURCE_LOADER - Celestial body name does not exist!" << std::endl;
		}

		return *bodyIt;
	}

	CelestialBody& GetBody(const int32_t inBodyID)
	{
		const auto& bodyIt = std::find_if(celestialBodiesRef.begin(), celestialBodiesRef.end(), [&inBodyID](const CelestialBody& celestialBody)
		{
			return celestialBody.ID == inBodyID;
		});

		if (bodyIt == celestialBodiesRef.end())
		{
			std::cout << "ERROR::RESOURCE_LOADER - Celestial body ID does not exist!" << std::endl;
		}

		return *bodyIt;
	}

	Shader& GetShader(const std::string& inShaderName)
	{
		const auto& shaderIt = std::find_if(shaders.begin(), shaders.end(), [&inShaderName](const Shader& inShader)
		{
			return inShader.GetEntityName() == inShaderName;
		});

		if (shaderIt == shaders.end())
		{
			std::cout << "ERROR::RESOURCE_LOADER - Shader name does not exist!" << std::endl;
		}

		return *shaderIt;
	}

	UniformBuffer & GetUBO(const std::string& inUniformName)
	{
		const auto& uboIt = std::find_if(ubos.begin(), ubos.end(), [&inUniformName](const UniformBuffer& inUBO)
		{
			return inUBO.GetUniformName() == inUniformName;
		});

		if (uboIt == ubos.end())
		{
			std::cout << "ERROR::RESOURCE_LOADER - Uniform name does not exist!" << std::endl;
		}

		return *uboIt;
	}
}