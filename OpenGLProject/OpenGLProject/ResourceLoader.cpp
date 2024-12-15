#include "ResourceLoader.h"

#include <algorithm>
#include <ctype.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
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
		shaders.emplace_back("CelestialBody", "DefaultShader.vs", "DefaultShader.fs");
		shaders.emplace_back("Sun", "DefaultShader.vs", "SunShader.fs");
		shaders.emplace_back("TextGlyph", "TextShader.vs", "TextShader.fs");
		shaders.emplace_back("BeltBody", "InstancedModelShader.vs", "DefaultShader.fs");
		shaders.emplace_back("MilkyWay", "SkyboxShader.vs", "SkyboxShader.fs");
		shaders.emplace_back("BodyRings", "DefaultShader.vs", "DefaultShader.fs");
		shaders.emplace_back("Orbit", "DefaultShader.vs", "DefaultShader.fs");

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

		std::string celestialBodyNameCache = "";

		const std::string& csvFile = Utils::ReadFile("../Data/CelestialBodyData.csv");
		std::string line;
		std::stringstream csvLineStream(csvFile);

		// Overwrite the first line (legend) by reading it before looping
		std::getline(csvLineStream, line, '\n');

		while (std::getline(csvLineStream, line, '\n'))
		{
			// @todo - Optimise...
			line.erase(line.find('\r'));

			std::stringstream csvFieldStream(line);
			std::string field;
			std::vector<std::string> celestialBodyParams;
			while (std::getline(csvFieldStream, field, ','))
			{
				celestialBodyParams.emplace_back(field);
			}

			const std::string& celestialBodyName = celestialBodyParams[0];
			const std::string& celestialBodyType = celestialBodyParams[1];
			const std::string& celestialBodyParentName = (celestialBodyType == "Moon") ? celestialBodyParams[8] : "";
			const float distanceToParent = std::stof(celestialBodyParams[3]);

			float scaledDistanceToParent = 0.0f;
			if (celestialBodyType == "Star")
			{
				scaledDistanceToParent = 0.0f;
			}
			else if (celestialBodyType == "Moon")
			{
				scaledDistanceToParent = GetBody(celestialBodyParentName).bodyData.radius + distanceToParent / sunEarthDistance * RADIUS_SCALE_FACTOR;
			}
			else
			{
				if (celestialBodyName == "Mercury")
				{
					scaledDistanceToParent = GetBody(celestialBodyNameCache).bodyData.radius * 2.0f + distanceToParent / sunEarthDistance * DISTANCE_SCALE_FACTOR;
				}
				else
				{
					scaledDistanceToParent = GetBody(celestialBodyNameCache).bodyData.distanceToParent + distanceToParent / sunEarthDistance * DISTANCE_SCALE_FACTOR;
				}
			}

			const std::filesystem::path& texturePath = assetPaths[celestialBodyName];
			const float scaledRadius = std::stof(celestialBodyParams[2]) / earthRadius * (celestialBodyType == "Star" ? 0.5f : 1.0f);
			const float obliquity = std::stof(celestialBodyParams[4]);
			const float scaledOrbitalPeriod = std::stof(celestialBodyParams[5]) * (celestialBodyType == "DwarfPlanet" ? GetBody("Earth").bodyData.orbitalPeriod : 1.0f);
			const float spinPeriod = std::stof(celestialBodyParams[6]);
			const float orbitalInclination = std::stof(celestialBodyParams[7]);
			const int32_t parentID = celestialBodyType == "Moon" ? GetBody(celestialBodyParentName).GetID() : -1;

			celestialBodiesRef.emplace_back(BodyData{ texturePath, scaledRadius, scaledDistanceToParent, obliquity, scaledOrbitalPeriod, spinPeriod, orbitalInclination, parentID });

			celestialBodyNameCache = celestialBodyName;
		}
	}

	void LoadBelts()
	{
		FillTextureMap("../Models/Belts/");

		const std::string& csvFile = Utils::ReadFile("../Data/BeltData.csv");
		std::string line;
		std::stringstream csvLineStream(csvFile);

		// Overwrite the first line (legend) by reading it before looping
		std::getline(csvLineStream, line, '\n');

		while (std::getline(csvLineStream, line, '\n'))
		{
			// @todo - Optimise...
			line.erase(line.find('\r'));

			std::stringstream csvFieldStream(line);
			std::string field;
			std::vector<std::string> beltParams;
			while (std::getline(csvFieldStream, field, ','))
			{
				beltParams.emplace_back(field);
			}

			const std::string& beltName = beltParams[0];
			const std::filesystem::path& modelPath = assetPaths[beltParams[1]];
			const uint32_t instanceCount = std::stoi(beltParams[2]);
			const float sizeRangeLowerBound = std::stof(beltParams[3]);
			const uint32_t sizeRangeSpan = std::stoi(beltParams[4]);
			const float outerBound = GetBody(beltParams[5]).bodyData.distanceToParent;
			const float innerBound = GetBody(beltParams[6]).bodyData.distanceToParent;
			float majorRadius = 0.0f;
			if (beltName == "AsteroidBelt")
			{
				majorRadius = innerBound * 1.05f + 0.5f * (outerBound * 0.9f - innerBound * 1.05f);
			}
			else
			{
				majorRadius = innerBound + 0.5f * (outerBound - innerBound);
			}
			float minorRadius = 0.0f;
			if (beltName == "AsteroidBelt")
			{
				minorRadius = 0.5f * (outerBound * 0.9f - innerBound * 1.05f);
			}
			else
			{
				minorRadius = 0.5f * (outerBound - innerBound);
			}
			const float flatnessFactor = std::stof(beltParams[7]);

			beltsRef.emplace_back(InstanceParams{ modelPath, instanceCount, sizeRangeLowerBound, sizeRangeSpan }, TorusParams{ majorRadius, minorRadius, flatnessFactor });
		}
	}

	CelestialBody& GetBody(const std::string& inBodyName)
	{
		const auto& bodyIt = std::find_if(celestialBodiesRef.begin(), celestialBodiesRef.end(), [&inBodyName](const CelestialBody& body)
		{
			return body.GetName() == inBodyName;
		});

		if (bodyIt == celestialBodiesRef.end())
		{
			std::cout << "ERROR::RESOURCE_LOADER - Celestial body " << inBodyName << " does not exist!" << std::endl;
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
			std::cout << "ERROR::RESOURCE_LOADER - Celestial body ID " << inBodyID << " does not exist!" << std::endl;
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
			std::cout << "ERROR::RESOURCE_LOADER - Shader " << inShaderName << " does not exist!" << std::endl;
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
			std::cout << "ERROR::RESOURCE_LOADER - Uniform " << inUniformName << " does not exist!" << std::endl;
		}

		return *uboIt;
	}
}