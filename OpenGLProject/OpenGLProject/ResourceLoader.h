#pragma once

#include <array>
#include <string>
#include <vector>

#include "CelestialBody.h"
#include "Shader.h"



namespace ResourceLoader
{
	constexpr uint32_t BODIES_COUNT = 18;
	constexpr float DIST_SCALE_FACTOR = 10.0f;

	// Paths to retrieve DDS textures (convention: size_name.dds)
	const std::array<const std::string, BODIES_COUNT> texturePaths
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

	CelestialBody& GetBody(const std::string& inBodyName);
	CelestialBody& GetBody(const int32_t inBodyID);
	Shader& GetShader(const std::string& inShaderName);

	void LoadShaders();

	// Fill data structures above with celestial bodies information/pre-computations (collected from NASA website, Wikipedia and passionates)
	// Note: data is scaled so we can visualise the whole Solar System without having to travel too much time.
	void LoadCelestialBodies();

	std::string GetNameFromTexturePath(const std::string& inTexturePath);
}