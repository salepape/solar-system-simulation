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
		shaders.emplace_back("BodyRings",		"DefaultShader.vs",			"DefaultShader.fs");
		shaders.emplace_back("Orbit",			"DefaultShader.vs",			"DefaultShader.fs");

		ubos.reserve(3);

		ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("Sun").GetRendererID(), GetShader("BeltBody").GetRendererID(), GetShader("BodyRings").GetRendererID(), GetShader("Orbit").GetRendererID(), GetShader("TextGlyph").GetRendererID(), GetShader("MilkyWay").GetRendererID() },
			"ubo_ProjectionView", Utils::mat4v4Size);
		ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("Sun").GetRendererID(), GetShader("BeltBody").GetRendererID(), GetShader("BodyRings").GetRendererID(), GetShader("Orbit").GetRendererID() },
			"ubo_CameraPosition", Utils::vec4Size);
		ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("BeltBody").GetRendererID(), GetShader("BodyRings").GetRendererID(), GetShader("Orbit").GetRendererID() },
			"ubo_PointLight", 4 * Utils::vec4Size + 4 * Utils::scalarSize);
	}

	void LoadCelestialBodies()
	{
		int index = 0;
		const int earthRadius = 6371;
		const long int sunEarthDistance = 149600000;

		// Planets
		/* Sun */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 696300.0f / earthRadius * 0.5f, 0.0f,																										7.25f,	 0.0f,		25.05f,		  0.0f  });
		/* Mercury */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 2439.7f / earthRadius,			 GetBody("Sun").bodyData.radius * 2.0f + 57900000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,			0.03f,   87.97f,	1407.6f / 24, 7.01f });
		/* Venus */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 6051.8f / earthRadius,			 GetBody("Mercury").bodyData.distanceToParent + 108200000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	2.64f,   224.7f,   -5832.5f / 24, 3.39f });
		/* Earth */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 1.0f,							 GetBody("Venus").bodyData.distanceToParent + 1.0f * DISTANCE_SCALE_FACTOR,									23.44f,  365.26f,   23.9f / 24,   0.0f  });
		/* Mars */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 3389.6f / earthRadius,			 GetBody("Earth").bodyData.distanceToParent + 228000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		25.19f,  686.97f,   24.6f / 24,   1.85f });
		/* Jupiter */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 69991.0f / earthRadius,		 GetBody("Mars").bodyData.distanceToParent + 778500000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		3.13f,   4332.59f,  9.9f / 24,	  1.31f });
		/* Saturn */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 58232.0f / earthRadius,		 GetBody("Jupiter").bodyData.distanceToParent + 1432000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	26.73f,  10759.22f, 10.7f / 24,   2.49f });
		/* Uranus */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 25362.0f / earthRadius,		 GetBody("Saturn").bodyData.distanceToParent + 2867000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	82.23f,  30688.5,  -17.2f / 24,   0.77f });
		/* Neptune */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 24622.0f / earthRadius,		 GetBody("Uranus").bodyData.distanceToParent + 4515000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	28.32f,  60182.0f,  16.1f / 24,   1.77f });

		// Dwarf planets
		/* Ceres */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 469.0f / earthRadius,	GetBody("Mars").bodyData.distanceToParent + 414000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		4.0f,    4.60f * GetBody("Earth").bodyData.orbitalPeriod,    9.07f / 24,	10.6f   });
		/* Orcus */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 460.0f / earthRadius,  GetBody("Neptune").bodyData.distanceToParent + 5890000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	0.0f,	 245.19f * GetBody("Earth").bodyData.orbitalPeriod,  10.5f / 24,	20.59f  });		// Rotation period very uncertain + orbit far from circular
		/* Pluto */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 1188.0f / earthRadius, GetBody("Orcus").bodyData.distanceToParent + 5910000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		119.51f, 247.94f * GetBody("Earth").bodyData.orbitalPeriod, -153.29f / 24,	17.16f  });
		/* Salacia */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 914.0f / earthRadius,  GetBody("Pluto").bodyData.distanceToParent + 6310000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		0.0f,	 273.98f * GetBody("Earth").bodyData.orbitalPeriod,  6.09f / 24,	23.921f });		// Axial tilt unknown
		/* Haumea */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 870.0f / earthRadius,  GetBody("Salacia").bodyData.distanceToParent + 6470000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	126.0f,  283.12f * GetBody("Earth").bodyData.orbitalPeriod,  3.92f / 24,	28.21f  });		// @todo - Haumea has an ovoidal shape in reality
		/* Quaoar */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 545.0f / earthRadius,  GetBody("Haumea").bodyData.distanceToParent + 6540000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		0.0f,	 288.83f * GetBody("Earth").bodyData.orbitalPeriod,  17.68f / 24,	7.99f   });		// Synodic rotation period used
		/* Makemake */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 358.0f / earthRadius,	GetBody("Quaoar").bodyData.distanceToParent + 6820000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		0.0f,	 306.21f * GetBody("Earth").bodyData.orbitalPeriod,  22.83f / 24,	28.98f  });
		/* Gonggong */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 615.0f / earthRadius,  GetBody("Makemake").bodyData.distanceToParent + 10100000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	0.0f,	 554.37f * GetBody("Earth").bodyData.orbitalPeriod,  22.40f / 24,	30.63f  });		// Synodic rotation period used
		/* Eris */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 1200.0f / earthRadius, GetBody("Gonggong").bodyData.distanceToParent + 10200000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	78.3f,	 559.07f * GetBody("Earth").bodyData.orbitalPeriod,  15.786f,		44.040f });
		/* Sedna */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 500.0f / earthRadius,  GetBody("Eris").bodyData.distanceToParent + 75800000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		0.0f,	 11390.0f * GetBody("Earth").bodyData.orbitalPeriod, 10.273f / 24,	11.93f  });		// Synodic rotation period used

		// Moons
		/* Luna */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 1737.1f / earthRadius, GetBody("Earth").bodyData.radius + 384400.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		6.68f,  27.32f,		27.32f,		5.145f,	   GetBody("Earth").GetID() });

		/* Io */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 1821.6f / earthRadius, GetBody("Jupiter").bodyData.radius + 421700.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   1.77f,		1.77f,		2.213f,	   GetBody("Jupiter").GetID() });
		/* Europa */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 1560.8f / earthRadius, GetBody("Jupiter").bodyData.radius + 670900.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.1f,   3.55f,		3.55f,		1.791f,	   GetBody("Jupiter").GetID() });
		/* Ganymede */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 2634.1f / earthRadius, GetBody("Jupiter").bodyData.radius + 1070400.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.16f,  7.15f,		7.15f,		2.214f,    GetBody("Jupiter").GetID() });
		/* Callisto */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 2410.3f / earthRadius, GetBody("Jupiter").bodyData.radius + 1883000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   16.69f,		16.69f,		2.017f,	   GetBody("Jupiter").GetID() });

		/* Mimas */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 198.2f / earthRadius,	GetBody("Saturn").bodyData.radius + 186000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,	0.942f,		0.942f,		1.574f,    GetBody("Saturn").GetID() });	// Inclination to Saturn equator
		/* Enceladus */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 252.1f / earthRadius,	GetBody("Saturn").bodyData.radius + 238000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,	1.370f,		1.370f,		0.009f,    GetBody("Saturn").GetID() });	// Inclination to Saturn equator
		/* Tethys */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 531.1f / earthRadius,	GetBody("Saturn").bodyData.radius + 295000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   1.89f,		1.89f,		1.12f,     GetBody("Saturn").GetID() });
		/* Dione */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 561.4f / earthRadius,	GetBody("Saturn").bodyData.radius + 377400.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   2.74f,		2.74f,		0.019f,    GetBody("Saturn").GetID() });
		/* Rhea */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 763.8f / earthRadius,	GetBody("Saturn").bodyData.radius + 527000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   4.52f,		4.52f,		0.345f,    GetBody("Saturn").GetID() });
		/* Titan */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 2575.5f / earthRadius, GetBody("Saturn").bodyData.radius + 1200000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   15.95f,		15.95f,		0.0f,      GetBody("Saturn").GetID() });
		/* Iapetus */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 734.5f / earthRadius,	GetBody("Saturn").bodyData.radius + 1221850.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   79.32f,		79.32f,		15.47f,    GetBody("Saturn").GetID() });

		/* Puck */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 81.0f / earthRadius,	GetBody("Uranus").bodyData.radius + 86010.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,	0.762f,		0.762f,		0.319f,    GetBody("Uranus").GetID() });	// Inclination to Uranus equator
		/* Miranda */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 235.8f / earthRadius,	GetBody("Uranus").bodyData.radius +	129900.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,	1.413f,		1.413f,		4.232f,    GetBody("Uranus").GetID() });	// Inclination to Uranus equator
		/* Ariel */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 578.9f / earthRadius,	GetBody("Uranus").bodyData.radius + 190000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   2.520f,		2.520f,		0.260f,    GetBody("Uranus").GetID() });
		/* Umbriel */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 584.7f / earthRadius,	GetBody("Uranus").bodyData.radius + 266000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   4.144f,		4.144f,		0.128f,    GetBody("Uranus").GetID() });
		/* Titania */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 788.4f / earthRadius,	GetBody("Uranus").bodyData.radius + 436000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   8.706f,		8.706f,		0.340f,    GetBody("Uranus").GetID() });
		/* Oberon */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 761.4f / earthRadius,	GetBody("Uranus").bodyData.radius + 584000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   13.463f,	13.463f,	0.058f,    GetBody("Uranus").GetID() });

		/* Larissa */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 97.0f / earthRadius,	GetBody("Neptune").bodyData.radius + 73600.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   0.555f,		0.555f,		0.251f,	   GetBody("Neptune").GetID() });	// Inclination to Neptune equator
		/* Proteus */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 209.0f / earthRadius,	GetBody("Neptune").bodyData.radius + 117600.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   1.122f,		1.122f,		0.524f,	   GetBody("Neptune").GetID() });	// Inclination to Neptune equator
		/* Triton */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 1353.4f / earthRadius,	GetBody("Neptune").bodyData.radius + 354800.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   5.88f,		5.88f,		129.812f,  GetBody("Neptune").GetID() });

		/* Vanth */		celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 221.5f / earthRadius,	GetBody("Orcus").bodyData.radius + 8980.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   9.539f,		9.539f,		90.54f,	   GetBody("Orcus").GetID() });		// occultation radius taken, not thermal radiation one; no axial tilt?; inclination to ecliptic

		/* Charon */	celestialBodiesRef.emplace_back(BodyData{ texturePaths[index++], 606.0f / earthRadius,	GetBody("Pluto").bodyData.radius + 19640.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   6.387f,		6.387f,		0.080f,	   GetBody("Pluto").GetID() });

		/* Actaea */

		/* Hi'iaka */
		/* Namaka */

		/* Weywot */

		/* MK2 */

		/* Xiangliu */

		/* Dysnomia */
	}

	void LoadBelts()
	{
		beltsRef.reserve(BELTS_COUNT);

		/* Asteroid belt */		beltsRef.emplace_back(InstanceParams{ "../Models/Asteroid.obj",  2500,		0.05f,	10 }, TorusParams{ GetBody("Mars").bodyData.distanceToParent * 1.05f + 0.5f * (GetBody("Jupiter").bodyData.distanceToParent * 0.9f - GetBody("Mars").bodyData.distanceToParent * 1.05f),	0.5f * (GetBody("Jupiter").bodyData.distanceToParent * 0.9f - GetBody("Mars").bodyData.distanceToParent * 1.05f),	0.4f });
		/* Kuiper belt */		beltsRef.emplace_back(InstanceParams{ "../Models/Ice.obj",		 25000,		0.05f,	20 }, TorusParams{ GetBody("Neptune").bodyData.distanceToParent + 0.5f * (GetBody("Makemake").bodyData.distanceToParent - GetBody("Neptune").bodyData.distanceToParent),					0.5f * (GetBody("Makemake").bodyData.distanceToParent - GetBody("Neptune").bodyData.distanceToParent),				0.4f });
		/* Scattered disc */	beltsRef.emplace_back(InstanceParams{ "../Models/Ice.obj",		 5000,		0.05f,	5  }, TorusParams{ GetBody("Gonggong").bodyData.distanceToParent + 0.5f * (GetBody("Sedna").bodyData.distanceToParent - GetBody("Gonggong").bodyData.distanceToParent),						0.5f * (GetBody("Sedna").bodyData.distanceToParent - GetBody("Gonggong").bodyData.distanceToParent),				0.4f });
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