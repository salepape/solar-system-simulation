#pragma once

#include <cstdint>
#include <string>

class BodyRings;
class BodySystem;
class Shader;
class UniformBuffer;



// @todo - Convert it to a pure static util class
namespace ResourceLoader
{
	BodySystem& GetBodySystem(const std::string& inBodyName);
	BodySystem& GetBodySystem(const int32_t inBodyID);

	BodyRings& GetBodyRings(const std::string& inBodyName);

	Shader& GetShader(const std::string& inShaderName);
	UniformBuffer& GetUBO(const std::string& inUniformName);

	// Build/compile shaders and their corresponding programs
	void LoadShaders();

	// @todo - Think about using a Builder Design Pattern to construct such class instances out of CSV files
	// Instantiate "spherical" celestial bodies/ring systems/belt systems, after loading data from .csv files,
	// and re-scaling it so we can visualise the whole Solar System without having to travel for too long
	void LoadCelestialBodies();
	void LoadRings();
	void LoadBelts();
}