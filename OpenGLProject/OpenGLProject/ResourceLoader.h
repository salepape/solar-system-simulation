#pragma once

#include <array>
#include <string>
#include <vector>

#include "Belt.h"
#include "CelestialBody.h"
#include "Shader.h"
#include "UniformBuffer.h"



namespace ResourceLoader
{
	constexpr uint32_t BODIES_COUNT = 49;
	constexpr float DISTANCE_SCALE_FACTOR = 10.0f;
	constexpr float RADIUS_SCALE_FACTOR = 1000.0f;
	constexpr uint32_t BELTS_COUNT = 2;

	// Paths to retrieve DDS textures (convention: size_name.dds)
	const std::array<const std::string, BODIES_COUNT> texturePaths
	{
		// Star (1)
		"../Textures/Stars/8k_sun.dds",

		// Planets (8)
		"../Textures/Planets/8k_mercury.dds",
		"../Textures/Planets/4k_venus_atmosphere.dds",				// "../Textures/Planets/8k_venus.dds"
		"../Textures/Planets/8k_earth.dds",							// "../Textures/Planets/8k_earth_normal/specular_map.dds"
		"../Textures/Planets/8k_mars.dds",
		"../Textures/Planets/8k_jupiter.dds",
		"../Textures/Planets/8k_saturn.dds",
		"../Textures/Planets/2k_uranus.dds",
		"../Textures/Planets/2k_neptune.dds",

		// Dwarf planets (10)
		"../Textures/DwarfPlanets/4k_ceres.dds",
		"../Textures/DwarfPlanets/2k_orcus.dds",
		"../Textures/DwarfPlanets/4k_pluto_enhanced_colours.dds",	// "../Textures/Planets/2k_pluto.dds"
		"../Textures/DwarfPlanets/2k_salacia.dds",
		"../Textures/DwarfPlanets/2k_haumea.dds",
		"../Textures/DwarfPlanets/2k_quaoar.dds",
		"../Textures/DwarfPlanets/4k_makemake.dds",
		"../Textures/DwarfPlanets/2k_gonggong.dds",
		"../Textures/DwarfPlanets/4k_eris.dds",
		"../Textures/DwarfPlanets/2k_sedna.dds",

		// Moons (30 = 1 + 4 + 7 + 6 + 3 + 1 + 1 + 1 + 2 + 1 + 1 + 1 + 1)
		// Earth
		"../Textures/Satellites/8k_luna.dds",

		// Mars
		//"../Textures/Satellites/1k_deimos.dds",
		//"../Textures/Satellites/2k_phobos.dds",

		// Jupiter
		"../Textures/Satellites/3k_io.dds",
		"../Textures/Satellites/2k_europa.dds",
		"../Textures/Satellites/2k_ganymede.dds",
		"../Textures/Satellites/2k_callisto.dds",

		// Saturn
		"../Textures/Satellites/1k_mimas.dds",
		"../Textures/Satellites/2k_enceladus.dds",
		"../Textures/Satellites/3k_tethys.dds",
		"../Textures/Satellites/2k_dione.dds",
		"../Textures/Satellites/2k_rhea.dds",
		"../Textures/Satellites/8k_titan.dds",
		"../Textures/Satellites/1k_iapetus.dds",

		// Uranus
		"../Textures/Satellites/2k_puck.dds",						// Copy of Rhea texture
		"../Textures/Satellites/1k_miranda.dds",
		"../Textures/Satellites/1k_ariel.dds",
		"../Textures/Satellites/1k_umbriel.dds",
		"../Textures/Satellites/1k_titania.dds",
		"../Textures/Satellites/1k_oberon.dds",

		// Neptune
		"../Textures/Satellites/2k_larissa.dds",					// Copy of Rhea texture
		"../Textures/Satellites/2k_proteus.dds",					// Copy of Rhea texture
		"../Textures/Satellites/1k_triton.dds",

		// Orcus
		"../Textures/Satellites/2k_vanth.dds",

		// Pluto
		"../Textures/Satellites/1k_charon.dds",

		// Salacia
		"../Textures/Satellites/2k_actaea.dds",

		// Haumea
		"../Textures/Satellites/2k_hi'iaka.dds",
		"../Textures/Satellites/2k_namaka.dds",						// Same texture used for Hi'iaka

		// Quaoar
		"../Textures/Satellites/2k_weywot.dds",

		// Makemake
		"../Textures/Satellites/2k_MK2.dds",

		// Gonggong
		"../Textures/Satellites/2k_xiangliu.dds",

		// Eris
		"../Textures/Satellites/2k_dysnomia.dds"
	};



	CelestialBody& GetBody(const std::string& inBodyName);
	CelestialBody& GetBody(const int32_t inBodyID);

	Shader& GetShader(const std::string& inShaderName);
	UniformBuffer& GetUBO(const std::string& inUniformName);

	// Build/compile shaders and their corresponding programs
	void LoadShaders();

	// @todo - Load raw data from .csv file
	// Fill data structures above with "spherical" celestial bodies information (collected from NASA website, Wikipedia and passionates)
	// Note: data is scaled so we can visualise the whole Solar System without having to travel too much time.
	void LoadCelestialBodies();
	void LoadBelts();

	std::string GetNameFromTexturePath(const std::string& inTexturePath);
}