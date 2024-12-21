#pragma once

#include <memory>
#include <string>

#include "BodyRings.h"
#include "CelestialBody.h"
#include "Shader.h"
#include "UniformBuffer.h"



// @todo - Make everything static in this class?
namespace ResourceLoader
{
	constexpr float DISTANCE_SCALE_FACTOR = 10.0f;
	constexpr float RADIUS_SCALE_FACTOR = 1000.0f;

	CelestialBody& GetBody(const std::string& inBodyName);
	CelestialBody& GetBody(const int32_t inBodyID);

	BodyRings& GetBodyRings(const std::string& inBodyName);

	Shader& GetShader(const std::string& inShaderName);
	UniformBuffer& GetUBO(const std::string& inUniformName);

	// Build/compile shaders and their corresponding programs
	void LoadShaders();

	// @todo - Load raw data from .csv file
	// Fill data structures above with "spherical" celestial bodies information (collected from NASA website, Wikipedia and passionates)
	// Note: data is scaled so we can visualise the whole Solar System without having to travel too much time.
	void LoadAssets();
	void LoadCelestialBodies();
	void LoadRings();
	void LoadBelts();

	std::string GetNameFromTexturePath(const std::filesystem::path& inTexturePath);
}