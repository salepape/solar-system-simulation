#include "ResourceLoader.h"

#include <algorithm>
#include <ctype.h>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Belt.h"
#include "SolarSystem.h"
#include "Utils.h"



std::vector<Shader> shaders;
std::vector<UniformBuffer> ubos;

namespace ResourceLoader
{
	// @todo - Use asset handles instead of strings for asset IDs
	// Paths to retrieve DDS textures (convention: size_name.dds)
	std::unordered_map<std::string, std::filesystem::path> assetPaths;

	std::vector<CelestialBody>& celestialBodiesRef = SolarSystem::GetCelestialBodiesVector();
	std::vector<Belt>& beltsRef = SolarSystem::GetBeltsVector();

	void FillTextureMap(const std::filesystem::path& inTexturePath)
	{
		for (const auto& directory : std::filesystem::recursive_directory_iterator(inTexturePath))
		{
			const std::filesystem::path& directoryPath = directory.path();
			const std::string& modelName = GetNameFromTexturePath(directoryPath);
			if (modelName == "")
			{
				continue;
			}

			assetPaths[modelName] = directoryPath;
		}
	}

	std::string GetNameFromTexturePath(const std::filesystem::path& inTexturePath)
	{
		std::string bodyFileName = inTexturePath.filename().string();

		size_t firstTrimSymbol = bodyFileName.find_first_of("_");
		if (firstTrimSymbol == std::string::npos)
		{
			firstTrimSymbol = bodyFileName.find_first_of(".");
			if (firstTrimSymbol == std::string::npos)
			{
				return "";
			}
		}
		std::string fileWithoutSize = firstTrimSymbol == bodyFileName.find_first_of("_") ? bodyFileName.substr(firstTrimSymbol + 1, bodyFileName.length() - firstTrimSymbol - 1) : bodyFileName.substr(0, firstTrimSymbol);

		size_t lastTrimSymbol = fileWithoutSize.find_first_of("_");
		if (lastTrimSymbol == std::string::npos)
		{
			lastTrimSymbol = fileWithoutSize.find_first_of(".");
		}
		std::string bodyName = fileWithoutSize.substr(0, lastTrimSymbol);

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

		ubos.reserve(5);

		ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("Sun").GetRendererID(), GetShader("BeltBody").GetRendererID(), GetShader("BodyRings").GetRendererID(), GetShader("Orbit").GetRendererID(), GetShader("TextGlyph").GetRendererID(), GetShader("MilkyWay").GetRendererID() },
			"ubo_ProjectionView", Utils::mat4v4Size);
		ubos.emplace_back(std::vector<uint32_t>{ GetShader("CelestialBody").GetRendererID(), GetShader("Sun").GetRendererID(), GetShader("BeltBody").GetRendererID(), GetShader("BodyRings").GetRendererID(), GetShader("Orbit").GetRendererID() },
			"ubo_CameraPosition", Utils::vec4Size);

		const std::vector<uint32_t> bodyShaderIDs{ GetShader("CelestialBody").GetRendererID(), GetShader("BeltBody").GetRendererID(), GetShader("BodyRings").GetRendererID(), GetShader("Orbit").GetRendererID() };
		ubos.emplace_back(bodyShaderIDs, "ubo_DirectionalLight", 4 * Utils::vec4Size + Utils::scalarSize);
		ubos.emplace_back(bodyShaderIDs, "ubo_PointLight", 4 * Utils::vec4Size + 4 * Utils::scalarSize);
		ubos.emplace_back(bodyShaderIDs, "ubo_SpotLight", 5 * Utils::vec4Size + 7 * Utils::scalarSize);
	}

	std::vector<std::filesystem::path> GetSubfolderPaths(const std::string& subfolder)
	{
		std::vector<std::filesystem::path> subfolderPaths;
		for (const auto& directory : std::filesystem::recursive_directory_iterator(std::filesystem::path(subfolder)))
		{
			subfolderPaths.emplace_back(directory.path());
		}

		return subfolderPaths;
	}

	void LoadAssets()
	{
		LoadCelestialBodies();
		LoadBelts();
	}

	void LoadCelestialBodies()
	{
		FillTextureMap("../Textures/CelestialBodies/");

		const int earthRadius = 6371;
		const long int sunEarthDistance = 149600000;

		// Stars
		const std::vector<std::filesystem::path>& starPath = GetSubfolderPaths("../Textures/CelestialBodies/Stars/");
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Sun"],		696300.0f / earthRadius * 0.5f, 0.0f,																								7.25f,	 0.0f,		25.05f,		  0.0f });

		// Planets
		const std::vector<std::filesystem::path>& planetPaths = GetSubfolderPaths("../Textures/CelestialBodies/Planets/");
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Mercury"],	2439.7f / earthRadius,	GetBody("Sun").bodyData.radius * 2.0f + 57900000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,				0.03f,   87.97f,	1407.6f / 24, 7.01f });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Venus"],		6051.8f / earthRadius,	GetBody("Mercury").bodyData.distanceToParent + 108200000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		2.64f,   224.7f,   -5832.5f / 24, 3.39f });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Earth"],		1.0f,					GetBody("Venus").bodyData.distanceToParent + 1.0f * DISTANCE_SCALE_FACTOR,									23.44f,  365.26f,   23.9f / 24,   0.0f });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Mars"],		3389.6f / earthRadius,	GetBody("Earth").bodyData.distanceToParent + 228000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		25.19f,  686.97f,   24.6f / 24,   1.85f });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Jupiter"],	69991.0f / earthRadius,	GetBody("Mars").bodyData.distanceToParent + 778500000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		3.13f,   4332.59f,  9.9f / 24,	  1.31f });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Saturn"],		58232.0f / earthRadius,	GetBody("Jupiter").bodyData.distanceToParent + 1432000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	26.73f,  10759.22f, 10.7f / 24,   2.49f });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Uranus"],		25362.0f / earthRadius,	GetBody("Saturn").bodyData.distanceToParent + 2867000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		82.23f,  30688.5,  -17.2f / 24,   0.77f });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Neptune"],	24622.0f / earthRadius,	GetBody("Uranus").bodyData.distanceToParent + 4515000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		28.32f,  60182.0f,  16.1f / 24,   1.77f });

		// Dwarf planets
		const std::vector<std::filesystem::path>& dwarfPlanetPaths = GetSubfolderPaths("../Textures/CelestialBodies/DwarfPlanets/");
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Ceres"],		469.0f / earthRadius,	GetBody("Mars").bodyData.distanceToParent + 414000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		4.0f,    4.60f * GetBody("Earth").bodyData.orbitalPeriod,    9.07f / 24,	10.6f });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Orcus"],		460.0f / earthRadius,	GetBody("Neptune").bodyData.distanceToParent + 5890000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	0.0f,	 245.19f * GetBody("Earth").bodyData.orbitalPeriod,  10.5f / 24,	20.59f });		// Rotation period very uncertain + orbit far from circular
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Pluto"],		1188.0f / earthRadius,	GetBody("Orcus").bodyData.distanceToParent + 5910000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		119.51f, 247.94f * GetBody("Earth").bodyData.orbitalPeriod, -153.29f / 24,	17.16f });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Salacia"],	914.0f / earthRadius,	GetBody("Pluto").bodyData.distanceToParent + 6310000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		0.0f,	 273.98f * GetBody("Earth").bodyData.orbitalPeriod,  6.09f / 24,	23.921f });		// Axial tilt unknown
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Haumea"],		870.0f / earthRadius,	GetBody("Salacia").bodyData.distanceToParent + 6470000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	126.0f,  283.12f * GetBody("Earth").bodyData.orbitalPeriod,  3.92f / 24,	28.21f });		// @todo - Haumea has an ovoidal shape in reality
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Quaoar"],		545.0f / earthRadius,	GetBody("Haumea").bodyData.distanceToParent + 6540000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		0.0f,	 288.83f * GetBody("Earth").bodyData.orbitalPeriod,  17.68f / 24,	7.99f });		// Synodic rotation period used
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Makemake"],	358.0f / earthRadius,	GetBody("Quaoar").bodyData.distanceToParent + 6820000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		0.0f,	 306.21f * GetBody("Earth").bodyData.orbitalPeriod,  22.83f / 24,	28.98f });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Gonggong"],	615.0f / earthRadius,	GetBody("Makemake").bodyData.distanceToParent + 10100000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	0.0f,	 554.37f * GetBody("Earth").bodyData.orbitalPeriod,  22.40f / 24,	30.63f });		// Synodic rotation period used
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Eris"],		1200.0f / earthRadius,	GetBody("Gonggong").bodyData.distanceToParent + 10200000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,	78.3f,	 559.07f * GetBody("Earth").bodyData.orbitalPeriod,  15.786f,		44.040f });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Sedna"],		500.0f / earthRadius,	GetBody("Eris").bodyData.distanceToParent + 75800000000.0f / sunEarthDistance * DISTANCE_SCALE_FACTOR,		0.0f,	 11390.0f * GetBody("Earth").bodyData.orbitalPeriod, 10.273f / 24,	11.93f });		// Synodic rotation period used

		// Moons
		const std::vector<std::filesystem::path>& moonPaths = GetSubfolderPaths("../Textures/CelestialBodies/Satellites/");
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Luna"],		1737.1f / earthRadius,	GetBody("Earth").bodyData.radius + 384400.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		6.68f,  27.32f,		27.32f,		5.145f,	   GetBody("Earth").GetID() });

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Io"],			1821.6f / earthRadius,	GetBody("Jupiter").bodyData.radius + 421700.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   1.77f,		1.77f,		2.213f,	   GetBody("Jupiter").GetID() });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Europa"],		1560.8f / earthRadius,	GetBody("Jupiter").bodyData.radius + 670900.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.1f,   3.55f,		3.55f,		1.791f,	   GetBody("Jupiter").GetID() });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Ganymede"],	2634.1f / earthRadius,	GetBody("Jupiter").bodyData.radius + 1070400.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.16f,  7.15f,		7.15f,		2.214f,    GetBody("Jupiter").GetID() });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Callisto"],	2410.3f / earthRadius,	GetBody("Jupiter").bodyData.radius + 1883000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   16.69f,		16.69f,		2.017f,	   GetBody("Jupiter").GetID() });

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Mimas"],		198.2f / earthRadius,	GetBody("Saturn").bodyData.radius + 186000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,	0.942f,		0.942f,		1.574f,    GetBody("Saturn").GetID() });	// Inclination to Saturn equator
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Enceladus"],	252.1f / earthRadius,	GetBody("Saturn").bodyData.radius + 238000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,	1.370f,		1.370f,		0.009f,    GetBody("Saturn").GetID() });	// Inclination to Saturn equator
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Tethys"],		531.1f / earthRadius,	GetBody("Saturn").bodyData.radius + 295000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   1.89f,		1.89f,		1.12f,     GetBody("Saturn").GetID() });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Dione"],		561.4f / earthRadius,	GetBody("Saturn").bodyData.radius + 377400.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   2.74f,		2.74f,		0.019f,    GetBody("Saturn").GetID() });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Rhea"],		763.8f / earthRadius,	GetBody("Saturn").bodyData.radius + 527000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   4.52f,		4.52f,		0.345f,    GetBody("Saturn").GetID() });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Titan"],		2575.5f / earthRadius,	GetBody("Saturn").bodyData.radius + 1200000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   15.95f,		15.95f,		0.0f,      GetBody("Saturn").GetID() });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Iapetus"],	734.5f / earthRadius,	GetBody("Saturn").bodyData.radius + 1221850.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   79.32f,		79.32f,		15.47f,    GetBody("Saturn").GetID() });

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Puck"],		81.0f / earthRadius,	GetBody("Uranus").bodyData.radius + 86010.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,	0.762f,		0.762f,		0.319f,    GetBody("Uranus").GetID() });	// Inclination to Uranus equator
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Miranda"],	235.8f / earthRadius,	GetBody("Uranus").bodyData.radius + 129900.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,	1.413f,		1.413f,		4.232f,    GetBody("Uranus").GetID() });	// Inclination to Uranus equator
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Ariel"],		578.9f / earthRadius,	GetBody("Uranus").bodyData.radius + 190000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   2.520f,		2.520f,		0.260f,    GetBody("Uranus").GetID() });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Umbriel"],	584.7f / earthRadius,	GetBody("Uranus").bodyData.radius + 266000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   4.144f,		4.144f,		0.128f,    GetBody("Uranus").GetID() });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Titania"],	788.4f / earthRadius,	GetBody("Uranus").bodyData.radius + 436000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   8.706f,		8.706f,		0.340f,    GetBody("Uranus").GetID() });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Oberon"],		761.4f / earthRadius,	GetBody("Uranus").bodyData.radius + 584000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   13.463f,	13.463f,	0.058f,    GetBody("Uranus").GetID() });

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Larissa"],	97.0f / earthRadius,	GetBody("Neptune").bodyData.radius + 73600.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   0.555f,		0.555f,		0.251f,	   GetBody("Neptune").GetID() });	// Inclination to Neptune equator
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Proteus"],	209.0f / earthRadius,	GetBody("Neptune").bodyData.radius + 117600.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   1.122f,		1.122f,		0.524f,	   GetBody("Neptune").GetID() });	// Inclination to Neptune equator
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Triton"],		1353.4f / earthRadius,	GetBody("Neptune").bodyData.radius + 354800.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   5.88f,		5.88f,		129.812f,  GetBody("Neptune").GetID() });

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Vanth"],		221.5f / earthRadius,	GetBody("Orcus").bodyData.radius + 7770.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   9.539f,		9.539f,		90.54f,	   GetBody("Orcus").GetID() });		// occultation radius taken, not thermal radiation one; no axial tilt?; inclination to ecliptic

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Charon"],		606.0f / earthRadius,	GetBody("Pluto").bodyData.radius + 19640.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   6.387f,		6.387f,		0.080f,	   GetBody("Pluto").GetID() });

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Actaea"],		150.0f / earthRadius,	GetBody("Salacia").bodyData.radius + 5619.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   5.494f,		5.494f,		23.59f,	   GetBody("Salacia").GetID() });

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Hi'iaka"],	160.0f / earthRadius,	GetBody("Haumea").bodyData.radius + 39300.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   49.12f,		9.8f / 24,	126.356f,  GetBody("Haumea").GetID() });
		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Namaka"],		85.0f / earthRadius,	GetBody("Haumea").bodyData.radius + 49500.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   18.28f,		18.28f,		113.013f,  GetBody("Haumea").GetID() });

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Weywot"],		100.0f / earthRadius,	GetBody("Quaoar").bodyData.radius + 13300.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   12.43f,		12.43f,		15.8f,	   GetBody("Quaoar").GetID() });

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["MK2"],		85.0f / earthRadius,	GetBody("Makemake").bodyData.radius + 20921.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   12.4f,		12.4f,		75.0f,	   GetBody("Makemake").GetID() });

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Xiangliu"],	50.0f / earthRadius,	GetBody("Gonggong").bodyData.radius + 15000.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,	0.0f,   25.22f,		25.22f,		83.08f,	   GetBody("Gonggong").GetID() });

		celestialBodiesRef.emplace_back(BodyData{ assetPaths["Dysnomia"],	308.0f / earthRadius,	GetBody("Eris").bodyData.radius + 37300.0f / sunEarthDistance * RADIUS_SCALE_FACTOR,		0.0f,   15.79f,		15.79f,		61.59f,	   GetBody("Eris").GetID() });
	}

	void LoadBelts()
	{
		FillTextureMap("../Models/");

		const std::vector<std::filesystem::path>& beltPaths = GetSubfolderPaths("../Models/");
		beltsRef.emplace_back(InstanceParams{ assetPaths["Asteroid"],	2500,		0.05f,	10 },	TorusParams{ GetBody("Mars").bodyData.distanceToParent * 1.05f + 0.5f * (GetBody("Jupiter").bodyData.distanceToParent * 0.9f - GetBody("Mars").bodyData.distanceToParent * 1.05f),		0.5f * (GetBody("Jupiter").bodyData.distanceToParent * 0.9f - GetBody("Mars").bodyData.distanceToParent * 1.05f),	0.4f });
		beltsRef.emplace_back(InstanceParams{ assetPaths["Ice"],		25000,		0.05f,	20 },	TorusParams{ GetBody("Neptune").bodyData.distanceToParent + 0.5f * (GetBody("Makemake").bodyData.distanceToParent - GetBody("Neptune").bodyData.distanceToParent),						0.5f * (GetBody("Makemake").bodyData.distanceToParent - GetBody("Neptune").bodyData.distanceToParent),				0.4f });
		beltsRef.emplace_back(InstanceParams{ assetPaths["Ice"],		5000,		0.05f,	5 },	TorusParams{ GetBody("Gonggong").bodyData.distanceToParent + 0.5f * (GetBody("Sedna").bodyData.distanceToParent - GetBody("Gonggong").bodyData.distanceToParent),						0.5f * (GetBody("Sedna").bodyData.distanceToParent - GetBody("Gonggong").bodyData.distanceToParent),				0.4f });
	}

	CelestialBody& GetBody(const std::string& inBodyName)
	{
		const auto& bodyIt = std::find_if(celestialBodiesRef.begin(), celestialBodiesRef.end(), [&inBodyName](const CelestialBody& body)
		{
			return body.GetName() == inBodyName;
		});

		if (bodyIt == celestialBodiesRef.end())
		{
			std::cout << "ERROR::RESOURCE_LOADER - Celestial body name does not exist!" << std::endl;
		}

		return *bodyIt;
	}

	CelestialBody& GetBody(const int32_t inBodyID)
	{
		const auto& bodyIt = std::find_if(celestialBodiesRef.begin(), celestialBodiesRef.end(), [&inBodyID](const CelestialBody& body)
		{
			return body.GetID() == inBodyID;
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