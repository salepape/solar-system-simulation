#pragma once

#include <memory>
#include <string>

#include "BodyRings.h"
#include "BodySystem.h"
#include "Shader.h"
#include "UniformBuffer.h"



// @todo - Make everything static in this class?
namespace ResourceLoader
{
	constexpr float DISTANCE_SCALE_FACTOR = 10.0f;
	constexpr float RADIUS_SCALE_FACTOR = 1000.0f;

	BodySystem& GetBodySystem(const std::string& inBodyName);
	BodySystem& GetBodySystem(const int32_t inBodyID);

	BodyRings& GetBodyRings(const std::string& inBodyName);

	Shader& GetShader(const std::string& inShaderName);
	UniformBuffer& GetUBO(const std::string& inUniformName);

	// Build/compile shaders and their corresponding programs
	void LoadShaders();

	// Instantiate "spherical" celestial bodies/ring systems/belt systems, after loading data from .csv files,
	// and re-scaling it so we can visualise the whole Solar System without having to travel for too long
	void LoadAssets();
	void LoadCelestialBodies();
	void LoadRings();
	void LoadBelts();

	// Get body name from texture path (by convention, texture name stored in folders follows: [num]k_[bodyName]_[bodyNameOptionalPrecisions])
	const std::string GetNameFromTexturePath(const std::filesystem::path& inTexturePath);
}