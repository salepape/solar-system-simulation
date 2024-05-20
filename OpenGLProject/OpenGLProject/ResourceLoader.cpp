#include "ResourceLoader.h"

#include <algorithm>
#include <ctype.h>
#include <iostream>
#include <memory>



std::vector<Shader> shaders;

// @todo - Move this vector in a Scene class with the other entities, and just keep resource loading stuff here (e.g. read values from .csv etc...)
std::vector<CelestialBody> celestialBodies;

namespace ResourceLoader
{
	CelestialBody& GetBody(const std::string& inBodyName)
	{
		const auto& bodyIt = std::find_if(celestialBodies.begin(), celestialBodies.end(), [&inBodyName](const CelestialBody& celestialBody)
		{
			return celestialBody.name == inBodyName;
		});

		if (bodyIt == celestialBodies.end())
		{
			std::cout << "ERROR::RESOURCE_LOADER - Celestial body name does not exist!" << std::endl;
		}

		return *bodyIt;
	}

	CelestialBody& GetBody(const int32_t inBodyID)
	{
		const auto& bodyIt = std::find_if(celestialBodies.begin(), celestialBodies.end(), [&inBodyID](const CelestialBody& celestialBody)
		{
			return celestialBody.ID == inBodyID;
		});

		if (bodyIt == celestialBodies.end())
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

	void LoadShaders()
	{
		shaders.reserve(7);

		// RendererID will be identical for all shaders if we do not instantiate them line by line before pushing them into the vector
		static Shader celestialBodyShader("CelestialBody", "DefaultShader.vs", "DefaultShader.fs");
		static Shader sunShader("Sun", "DefaultShader.vs", "SunShader.fs");
		static Shader textShader("Text", "TextShader.vs", "TextShader.fs");
		static Shader beltBodyShader("BeltBody", "InstancedModelShader.vs", "DefaultShader.fs");
		static Shader skyboxShader("Skybox", "SkyboxShader.vs", "SkyboxShader.fs");
		static Shader saturnRingsShader("SaturnRings", "DefaultShader.vs", "DefaultShader.fs");
		static Shader orbitShader("Orbit", "DefaultShader.vs", "DefaultShader.fs");
		shaders.push_back(celestialBodyShader);
		shaders.push_back(sunShader);
		shaders.push_back(textShader);
		shaders.push_back(beltBodyShader);
		shaders.push_back(skyboxShader);
		shaders.push_back(saturnRingsShader);
		shaders.push_back(orbitShader);
	}

	// Fill data structures above with celestial bodies information/pre-computations (collected from NASA website, Wikipedia and passionates)
	// Note: data is scaled so we can visualise the whole Solar System without having to travel too much time.
	void LoadCelestialBodies()
	{
		celestialBodies.reserve(BODIES_COUNT);

		/* Sun */		celestialBodies.push_back({ texturePaths[0], 109.3f * 0.5f, 0.0f, 7.25f, 0.0f, 27.0f, 0.0f });
		/* Mercury */	celestialBodies.push_back({ texturePaths[1],  0.383f, GetBody("Sun").radius * 2.0f + 0.38f * DIST_SCALE_FACTOR,			0.03f,  87.97f,	   58.6f,   7.01f });
		/* Venus */		celestialBodies.push_back({ texturePaths[2],  0.95f,  GetBody("Mercury").distanceToParent + 0.72f * DIST_SCALE_FACTOR,	2.64f,  224.7f,   -243.02f, 3.39f });
		/* Earth */		celestialBodies.push_back({ texturePaths[3],  1.0f,	  GetBody("Venus").distanceToParent + 1.0f * DIST_SCALE_FACTOR,		23.44f, 365.26f,   0.99f,   0.0f });
		/* Mars */		celestialBodies.push_back({ texturePaths[4],  0.532f, GetBody("Earth").distanceToParent + 1.52f * DIST_SCALE_FACTOR,	25.19f, 686.97f,   1.03f,   1.85f });
		/* Jupiter */	celestialBodies.push_back({ texturePaths[5],  10.97f, GetBody("Mars").distanceToParent + 5.19f * DIST_SCALE_FACTOR,		3.13f,  4332.59f,  0.41f,   1.31f });
		/* Saturn */	celestialBodies.push_back({ texturePaths[6],  9.14f,  GetBody("Jupiter").distanceToParent + 9.53f * DIST_SCALE_FACTOR,	26.73f, 10759.22f, 0.43f,   2.49f });
		/* Uranus */	celestialBodies.push_back({ texturePaths[7],  3.981f, GetBody("Saturn").distanceToParent + 19.20f * DIST_SCALE_FACTOR,	82.23f, 30688.5,  -0.72f,   0.77f });
		/* Neptune */	celestialBodies.push_back({ texturePaths[8],  3.865f, GetBody("Uranus").distanceToParent + 30.05f * DIST_SCALE_FACTOR,	28.32f, 60182.0f,  0.67f,   1.77f });

		/* Ceres */		celestialBodies.push_back({ texturePaths[9],  0.074f, GetBody("Mars").distanceToParent + 2.75f * DIST_SCALE_FACTOR,		0.0f,   1683.15f,  9.07f,   17.14f });
		/* Pluto */		celestialBodies.push_back({ texturePaths[10], 0.186f, GetBody("Neptune").distanceToParent + 39.24f * DIST_SCALE_FACTOR,	57.47f, 90560.0f, -6.39f,   10.62f });

		/* Luna */		celestialBodies.push_back({ texturePaths[11], 0.273f, GetBody("Earth").radius + 0.384f * DIST_SCALE_FACTOR,				6.68f,  27.32f,    27.32f,  5.145f,	   GetBody("Earth").ID });
		/* Callisto */	celestialBodies.push_back({ texturePaths[12], 0.378f, GetBody("Jupiter").radius + 1.883f * DIST_SCALE_FACTOR,			0.0f,   16.69f,    16.69f,  2.017f,	   GetBody("Jupiter").ID });
		/* Europa */	celestialBodies.push_back({ texturePaths[13], 0.245f, GetBody("Jupiter").radius + 0.671f * DIST_SCALE_FACTOR,			0.1f,   3.55f,     3.55f,   1.791f,	   GetBody("Jupiter").ID });
		/* Ganymede */	celestialBodies.push_back({ texturePaths[14], 0.413f, GetBody("Jupiter").radius + 1.07f * DIST_SCALE_FACTOR,			0.16f,  7.15f,     7.15f,   2.214f,    GetBody("Jupiter").ID });
		/* Io */		celestialBodies.push_back({ texturePaths[15], 0.286f, GetBody("Jupiter").radius + 0.422f * DIST_SCALE_FACTOR,			0.0f,   1.77f,     1.77f,   2.213f,	   GetBody("Jupiter").ID });
		/* Titan */		celestialBodies.push_back({ texturePaths[16], 0.404f, GetBody("Saturn").radius + 1.222f * DIST_SCALE_FACTOR,			0.0f,   15.95f,    15.95f,  0.0f,      GetBody("Saturn").ID });
		/* Triton */	celestialBodies.push_back({ texturePaths[17], 0.212f, GetBody("Neptune").radius + 0.354f * DIST_SCALE_FACTOR,			0.0f,   5.88f,     5.88f,   129.812f,  GetBody("Neptune").ID });
		// /* Deimos */	celestialBodies.push_back({ texturePaths[18], 0.000f, GetBody("Mars").radius + 0.156f * DIST_SCALE_FACTOR,				0.0f,   1.263f,    1.263f,	27.58f,	   4 });
		// /* Phobos */	celestialBodies.push_back({ texturePaths[19], 0.000f, GetBody("Mars").radius + 0.94f * DIST_SCALE_FACTOR,				0.0f,	0.319f,    0.319f,  28.4f,	   4 });
	}

	std::string GetNameFromTexturePath(const std::string& inTexturePath)
	{
		std::string bodyNameLocal;
		bodyNameLocal.reserve(inTexturePath.size());

		auto it = inTexturePath.begin();
		for (it; *it != '_'; ++it);

		++it;
		for (it; *it != '_' && *it != '.'; ++it)
		{
			bodyNameLocal.push_back(*it);
		}

		bodyNameLocal[0] = toupper(bodyNameLocal[0]);

		return bodyNameLocal;
	}
}