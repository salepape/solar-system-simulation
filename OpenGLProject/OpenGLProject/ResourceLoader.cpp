#include "ResourceLoader.h"

#include <algorithm>
#include <ctype.h>
#include <iostream>
#include <utility>

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
		shaders.emplace_back("CelestialBody",	"DefaultShader.vs",			"DefaultShader.fs");
		shaders.emplace_back("Sun",				"DefaultShader.vs",			"SunShader.fs");
		shaders.emplace_back("TextGlyph",		"TextShader.vs",			"TextShader.fs");
		shaders.emplace_back("BeltBody",		"InstancedModelShader.vs",	"DefaultShader.fs");
		shaders.emplace_back("MilkyWay",		"SkyboxShader.vs",			"SkyboxShader.fs");
		shaders.emplace_back("SaturnRings",		"DefaultShader.vs",			"DefaultShader.fs");
		shaders.emplace_back("Orbit",			"DefaultShader.vs",			"DefaultShader.fs");

		ubos.reserve(3);

		ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("Sun").GetRendererID(), GetShader("BeltBody").GetRendererID(), GetShader("SaturnRings").GetRendererID(), GetShader("Orbit").GetRendererID(), GetShader("TextGlyph").GetRendererID(), GetShader("MilkyWay").GetRendererID() },
			"ubo_ProjectionView", Utils::mat4v4Size);
		ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("Sun").GetRendererID(), GetShader("BeltBody").GetRendererID(), GetShader("SaturnRings").GetRendererID(), GetShader("Orbit").GetRendererID() },
			"ubo_CameraPosition", Utils::vec4Size);
		ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("BeltBody").GetRendererID(), GetShader("SaturnRings").GetRendererID(), GetShader("Orbit").GetRendererID() },
			"ubo_PointLightParams", 4 * Utils::vec4Size + 4 * Utils::scalarSize);
	}

	void LoadCelestialBodies()
	{
		celestialBodiesRef.reserve(BODIES_COUNT);

		/* Sun */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[0],  109.3f * 0.5f, 0.0f, 7.25f, 0.0f, 27.0f, 0.0f });
		/* Mercury */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[1],  0.383f, GetBody("Sun").bodyData.radius * 2.0f + 0.38f * DIST_SCALE_FACTOR,			0.03f,   87.97f,	   58.6f,   7.01f });
		/* Venus */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[2],  0.95f,  GetBody("Mercury").bodyData.distanceToParent + 0.72f * DIST_SCALE_FACTOR,	2.64f,   224.7f,   -243.02f, 3.39f });
		/* Earth */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[3],  1.0f,   GetBody("Venus").bodyData.distanceToParent + 1.0f * DIST_SCALE_FACTOR,		23.44f,  365.26f,   0.99f,   0.0f  });
		/* Mars */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[4],  0.532f, GetBody("Earth").bodyData.distanceToParent + 1.52f * DIST_SCALE_FACTOR,		25.19f,  686.97f,   1.03f,   1.85f });
		/* Jupiter */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[5],  10.97f, GetBody("Mars").bodyData.distanceToParent + 5.19f * DIST_SCALE_FACTOR,		3.13f,   4332.59f,  0.41f,   1.31f });
		/* Saturn */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[6],  9.14f,  GetBody("Jupiter").bodyData.distanceToParent + 9.53f * DIST_SCALE_FACTOR,	26.73f,  10759.22f, 0.43f,   2.49f });
		/* Uranus */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[7],  3.981f, GetBody("Saturn").bodyData.distanceToParent + 19.20f * DIST_SCALE_FACTOR,	82.23f,  30688.5,  -0.72f,   0.77f });
		/* Neptune */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[8],  3.865f, GetBody("Uranus").bodyData.distanceToParent + 30.05f * DIST_SCALE_FACTOR,	28.32f,  60182.0f,  0.67f,   1.77f });

		/* Ceres */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[9],  939.4f / 6367,  GetBody("Mars").bodyData.distanceToParent + 420000000.0f / 149600000 * DIST_SCALE_FACTOR,		4.0f,    4.60f * GetBody("Earth").bodyData.orbitalPeriod,   9.07f,   10.6f   });
		/* Pluto */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[10], 1188.0f / 6367, GetBody("Neptune").bodyData.distanceToParent + 6000000000.0f / 149600000 * DIST_SCALE_FACTOR,	119.51f, 247.94f * GetBody("Earth").bodyData.orbitalPeriod, -6.39f,  17.16f  });
		/* Eris */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[11], 2400.0f / 6367, GetBody("Neptune").bodyData.distanceToParent + 10000000000.0f / 149600000 * DIST_SCALE_FACTOR,	78.3f,	 559.07f * GetBody("Earth").bodyData.orbitalPeriod, 15.786f, 44.040f }),
		/* Haumea */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[12], 1740.0f / 6367, GetBody("Neptune").bodyData.distanceToParent + 6500000000.0f / 149600000 * DIST_SCALE_FACTOR,	126.0f,  283.12f * GetBody("Earth").bodyData.orbitalPeriod, 3.92f,	 28.21f  }),
		/* Makemake */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[13], 715.0f / 6367,  GetBody("Neptune").bodyData.distanceToParent + 6847000000.0f / 149600000 * DIST_SCALE_FACTOR,	0.0f,	 306.21f * GetBody("Earth").bodyData.orbitalPeriod, 22.83f,  28.98f  }),

		/* Luna */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[14], 0.273f, GetBody("Earth").bodyData.radius + 0.384f * DIST_SCALE_FACTOR,				6.68f,  27.32f,    27.32f,  5.145f,	   GetBody("Earth").GetID()   });
		/* Callisto */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[15], 0.378f, GetBody("Jupiter").bodyData.radius + 1.883f * DIST_SCALE_FACTOR,			0.0f,   16.69f,    16.69f,  2.017f,	   GetBody("Jupiter").GetID() });
		/* Europa */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[16], 0.245f, GetBody("Jupiter").bodyData.radius + 0.671f * DIST_SCALE_FACTOR,			0.1f,   3.55f,     3.55f,   1.791f,	   GetBody("Jupiter").GetID() });
		/* Ganymede */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[17], 0.413f, GetBody("Jupiter").bodyData.radius + 1.07f * DIST_SCALE_FACTOR,				0.16f,  7.15f,     7.15f,   2.214f,    GetBody("Jupiter").GetID() });
		/* Io */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[18], 0.286f, GetBody("Jupiter").bodyData.radius + 0.422f * DIST_SCALE_FACTOR,			0.0f,   1.77f,     1.77f,   2.213f,	   GetBody("Jupiter").GetID() });
		/* Titan */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[19], 0.404f, GetBody("Saturn").bodyData.radius + 1.222f * DIST_SCALE_FACTOR,				0.0f,   15.95f,    15.95f,  0.0f,      GetBody("Saturn").GetID()  });
		/* Triton */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[20], 0.212f, GetBody("Neptune").bodyData.radius + 0.354f * DIST_SCALE_FACTOR,			0.0f,   5.88f,     5.88f,   129.812f,  GetBody("Neptune").GetID() });
		///* Deimos */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[22], 0.000f, GetBody("Mars").bodyData.radius + 0.156f * DIST_SCALE_FACTOR,				0.0f,   1.263f,    1.263f,	27.58f,	   GetBody("Mars").GetID()	  });
		///* Phobos */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[23], 0.000f, GetBody("Mars").bodyData.radius + 0.94f * DIST_SCALE_FACTOR,				0.0f,	0.319f,    0.319f,  28.4f,	   GetBody("Mars").GetID()	  });
	}

	void LoadBelts()
	{
		beltsRef.reserve(BELTS_COUNT);

		beltsRef.emplace_back(InstanceParams{ "../Models/Asteroid.obj",  5000,	0.05f,	10 }, TorusParams{ GetBody("Mars").bodyData.distanceToParent * 1.1f, 2.75f * ResourceLoader::DIST_SCALE_FACTOR * 1.0f / 2.5f, 0.4f });
		beltsRef.emplace_back(InstanceParams{ "../Models/Ice.obj",		 20000,	0.05f,	20 }, TorusParams{ GetBody("Neptune").bodyData.distanceToParent * 1.4f, 30.05f * ResourceLoader::DIST_SCALE_FACTOR, 0.4f });
	}

	CelestialBody& GetBody(const std::string& inBodyName)
	{
		const auto& bodyIt = std::find_if(celestialBodiesRef.begin(), celestialBodiesRef.end(), [&inBodyName](const CelestialBody& celestialBody)
		{
			return celestialBody.GetName() == inBodyName;
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
			return celestialBody.GetID() == inBodyID;
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