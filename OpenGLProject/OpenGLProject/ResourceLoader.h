#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "CelestialBody.h"

namespace ResourceLoader
{
	constexpr uint32_t BODIES_COUNT = 18;
	constexpr float DIST_SCALE_FACTOR = 10.0f;

	// Paths to retrieve DDS textures (convention: size_name.dds)
	static std::array<const std::string, BODIES_COUNT> texturePaths
	{
		// Star
		"../Textures/Stars/8k_sun.dds",

		// Planets
		"../Textures/Planets/8k_mercury.dds",
		"../Textures/Planets/4k_venus_atmosphere.dds",		//"../Textures/Planets/8k_venus.dds"
		"../Textures/Planets/8k_earth.dds",
		"../Textures/Planets/8k_mars.dds",
		"../Textures/Planets/8k_jupiter.dds",
		"../Textures/Planets/8k_saturn.dds",
		"../Textures/Planets/2k_uranus.dds",
		"../Textures/Planets/2k_neptune.dds",

		// Dwarf planets
		"../Textures/Planets/4k_ceres.dds",
		"../Textures/Planets/2k_pluto.dds",

		// Moons
		"../Textures/Satellites/8k_luna.dds",
		"../Textures/Satellites/2k_callisto.dds",
		"../Textures/Satellites/2k_europa.dds",
		"../Textures/Satellites/2k_ganymede.dds",
		"../Textures/Satellites/3k_io.dds",
		"../Textures/Satellites/8k_titan.dds",
		"../Textures/Satellites/1k_triton.dds"
		//"../Textures/Satellites/1k_deimos.dds",
		//"../Textures/Satellites/2k_phobos.dds",

		// Normal maps
		//"../Textures/Planets/8k_earth_normal_map.png"
	};



	// @todo - Move this vector in a Scene class with the other entities, and just keep resource loading stuff here (e.g. read values from .csv etc...)
	static std::vector<CelestialBody> celestialBodies;

	static auto& GetBodyFromName(const std::string& inBodyName)
	{
		const auto& bodyIt = std::find_if(celestialBodies.begin(), celestialBodies.end(), [&inBodyName](const CelestialBody& celestialBody)
		{
			return celestialBody.bodyName == inBodyName;
		});

		if (bodyIt == celestialBodies.end())
		{
			std::cout << "ERROR - Celestial body name does not exist!" << std::endl;
		}

		return *bodyIt;
	}

	static auto& GetBodyFromID(const int32_t inBodyID)
	{
		const auto& bodyIt = std::find_if(celestialBodies.begin(), celestialBodies.end(), [&inBodyID](const CelestialBody& celestialBody)
		{
			return celestialBody.bodyID == inBodyID;
		});

		if (bodyIt == celestialBodies.end())
		{
			std::cout << "ERROR - Celestial body ID does not exist!" << std::endl;
		}

		return *bodyIt;
	}

	// Fill data structures above with celestial bodies information/pre-computations (collected from NASA website, Wikipedia and passionates)
	// Note: data is scaled so we can visualise the whole Solar System without having to travel too much time.
	static void LoadCelestialBodies()
	{
		celestialBodies.reserve(BODIES_COUNT);

		/* Sun */ celestialBodies.push_back({ texturePaths[0], 109.3f * 0.5f, 0.0f, 7.25f, 0.0f, 27.0f, 0.0f });

		/* Mercury */	celestialBodies.push_back({ texturePaths[1],  0.383f, GetBodyFromName("Sun").radius * 2.0f + 0.38f * DIST_SCALE_FACTOR,			0.03f,  87.97f,	   58.6f,   7.01f });
		/* Venus */		celestialBodies.push_back({ texturePaths[2],  0.95f,  GetBodyFromName("Mercury").distanceToParent + 0.72f * DIST_SCALE_FACTOR,	2.64f,  224.7f,   -243.02f, 3.39f });
		/* Earth */		celestialBodies.push_back({ texturePaths[3],  1.0f,	  GetBodyFromName("Venus").distanceToParent + 1.0f * DIST_SCALE_FACTOR,		23.44f, 365.26f,   0.99f,   0.0f });
		/* Mars */		celestialBodies.push_back({ texturePaths[4],  0.532f, GetBodyFromName("Earth").distanceToParent + 1.52f * DIST_SCALE_FACTOR,	25.19f, 686.97f,   1.03f,   1.85f });
		/* Jupiter */	celestialBodies.push_back({ texturePaths[5],  10.97f, GetBodyFromName("Mars").distanceToParent + 5.19f * DIST_SCALE_FACTOR,		3.13f,  4332.59f,  0.41f,   1.31f });
		/* Saturn */	celestialBodies.push_back({ texturePaths[6],  9.14f,  GetBodyFromName("Jupiter").distanceToParent + 9.53f * DIST_SCALE_FACTOR,	26.73f, 10759.22f, 0.43f,   2.49f });
		/* Uranus */	celestialBodies.push_back({ texturePaths[7],  3.981f, GetBodyFromName("Saturn").distanceToParent + 19.20f * DIST_SCALE_FACTOR,	82.23f, 30688.5,  -0.72f,   0.77f });
		/* Neptune */	celestialBodies.push_back({ texturePaths[8],  3.865f, GetBodyFromName("Uranus").distanceToParent + 30.05f * DIST_SCALE_FACTOR,	28.32f, 60182.0f,  0.67f,   1.77f });

		/* Ceres */		celestialBodies.push_back({ texturePaths[9],  0.074f, GetBodyFromName("Mars").distanceToParent + 2.75f * DIST_SCALE_FACTOR,		0.0f,   1683.15f,  9.07f,   17.14f });
		/* Pluto */		celestialBodies.push_back({ texturePaths[10], 0.186f, GetBodyFromName("Neptune").distanceToParent + 39.24f * DIST_SCALE_FACTOR,	57.47f, 90560.0f, -6.39f,   10.62f });

		/* Luna */		celestialBodies.push_back({ texturePaths[11], 0.273f, GetBodyFromName("Earth").radius + 0.384f * DIST_SCALE_FACTOR,				6.68f,  27.32f,    27.32f,  5.145f,	   GetBodyFromName("Earth").bodyID });
		/* Callisto */	celestialBodies.push_back({ texturePaths[12], 0.378f, GetBodyFromName("Jupiter").radius + 1.883f * DIST_SCALE_FACTOR,			0.0f,   16.69f,    16.69f,  2.017f,	   GetBodyFromName("Jupiter").bodyID });
		/* Europa */	celestialBodies.push_back({ texturePaths[13], 0.245f, GetBodyFromName("Jupiter").radius + 0.671f * DIST_SCALE_FACTOR,			0.1f,   3.55f,     3.55f,   1.791f,	   GetBodyFromName("Jupiter").bodyID });
		/* Ganymede */	celestialBodies.push_back({ texturePaths[14], 0.413f, GetBodyFromName("Jupiter").radius + 1.07f * DIST_SCALE_FACTOR,			0.16f,  7.15f,     7.15f,   2.214f,    GetBodyFromName("Jupiter").bodyID });
		/* Io */		celestialBodies.push_back({ texturePaths[15], 0.286f, GetBodyFromName("Jupiter").radius + 0.422f * DIST_SCALE_FACTOR,			0.0f,   1.77f,     1.77f,   2.213f,	   GetBodyFromName("Jupiter").bodyID });
		/* Titan */		celestialBodies.push_back({ texturePaths[16], 0.404f, GetBodyFromName("Saturn").radius + 1.222f * DIST_SCALE_FACTOR,			0.0f,   15.95f,    15.95f,  0.0f,      GetBodyFromName("Saturn").bodyID });
		/* Triton */	celestialBodies.push_back({ texturePaths[17], 0.212f, GetBodyFromName("Neptune").radius + 0.354f * DIST_SCALE_FACTOR,			0.0f,   5.88f,     5.88f,   129.812f,  GetBodyFromName("Neptune").bodyID });
		// /* Deimos */	celestialBodies.push_back({ texturePaths[18], 0.000f, GetBodyFromName("Mars").radius + 0.156f * DIST_SCALE_FACTOR,				0.0f,   1.263f,    1.263f,	27.58f,	   4 });
		// /* Phobos */	celestialBodies.push_back({ texturePaths[19], 0.000f, GetBodyFromName("Mars").radius + 0.94f * DIST_SCALE_FACTOR,				0.0f,	0.319f,    0.319f,  28.4f,	   4 });
	}
}