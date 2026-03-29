#include "SolarSystem.h"

#include <algorithm>
#include <cstdint>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <map>
#include <unordered_map>

#include "Application.h"
#include "Billboard.h"
#include "BodyRings.h"
#include "CelestialBody.h"
#include "PerspectiveCamera.h"
#include "PerspectiveCameraController.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "TextRenderer.h"
#include "Utils.h"
#include "Window.h"



SolarSystem::SolarSystem() :
	milkyWay({ "../Textures/MilkyWay/stars.dds" })
{
	BuildBodySystems();

	// @todo - Should not be called from here ideally
	BuildBodyRings();

	BuildBelts();

	for (const BodySystem& bodySystem : bodySystems)
	{
		// Texture creation is handled by the Text Renderer for now (glyph rendering issue when textures created otherwise)
		textRenderer.LoadASCIICharacters(bodySystem.celestialBody.GetName());
	}
	// Free FT resources once we don't have any more letters to load
	textRenderer.FreeFTResources();

	// Render the whole scene as long as the user is in the sphere of center 'Sun position' and radius 'distance Sun - farthest celestial body'
	spacecraft = std::make_shared<Spacecraft>(glm::vec3(0.0f, GetBodySystem("Sun").celestialBody.bodyData.radius * 1.75f, -25.0f), glm::vec3(0.0f, -25.0f, 90.0f), 45.0f, 2.0f * GetBodySystem("Sedna").celestialBody.bodyData.distanceToParent);
}

void SolarSystem::Update()
{
	renderer.Clear();

	const Application& app = runningApp;

	PerspectiveCameraController& cameraController = spacecraft->cameraController;
	cameraController.ProcessKeyboardInput(runningApp.deltaTime);

	PerspectiveCamera& camera = cameraController.GetCamera();
	camera.SetProjectionViewVUniform(openWindow.GetAspectRatio());
	camera.SetPositionFUniform();
	const glm::vec3& cameraPosition = camera.GetPosition();

	// Compute celestial body positions and sort them from farthest to closest according to the camera, 
	// because it is needed to make blending work for multiple objects with transparency, like body names and Saturn Rings
	std::map<float, BodySystem&> bodiesSortedByDistance;
	for (BodySystem& bodySystem : bodySystems)
	{
		const CelestialBody* parentBody = bodySystem.celestialBody.bodyData.parentName != "" ? &GetBodySystem(bodySystem.celestialBody.bodyData.parentName).celestialBody : nullptr;
		bodySystem.celestialBody.ComputeCartesianPosition(app.elapsedTime * app.speedFactor, parentBody);

		bodiesSortedByDistance.insert({
			glm::distance(cameraPosition, bodySystem.celestialBody.GetPosition()),
			bodySystem });
	}

	// @todo - Shouldn't need to update anything else than text orientation when legend is displayed

	// Draw all the body systems of the Solar System
	for (auto bodyIt = bodiesSortedByDistance.rbegin(); bodyIt != bodiesSortedByDistance.rend(); ++bodyIt)
	{
		// Draw celestial bodies, and animate them accordingly over time
		BodySystem& bodySystem = bodyIt->second;
		const glm::vec3& parentPosition = bodySystem.celestialBody.bodyData.parentName != "" ? GetBodySystem(bodySystem.celestialBody.bodyData.parentName).celestialBody.GetPosition() : glm::vec3(0.0f, 0.0f, 0.0f);
		bodySystem.Render(renderer, app.IsLegendDisplayed(), textRenderer, camera, parentPosition, app.elapsedTime * app.speedFactor);
	}

	// Draw the 2 main belts of the Solar System
	for (Belt& belt : belts)
	{
		belt.Render(renderer);
	}

	// Draw Milky Way skybox
	camera.SetInfiniteProjectionViewVUniform(openWindow.GetAspectRatio());
	milkyWay.Render(renderer);
}

void SolarSystem::BuildBodySystems()
{
	// @todo - First number to be replaced by an access from the .csv file
	constexpr long int sunEarthDistance = 149600000;
	constexpr float DISTANCE_SCALE_FACTOR = 10.0f;

	// @todo - First number to be replaced by an access from the .csv file
	constexpr int earthRadius = 6371;
	constexpr float RADIUS_SCALE_FACTOR = 1000.0f;

	// Needed to scale distance of current celestial body according to the previous one (diverging from proper simulation here, for travel end-user convenience)
	std::string celestialBodyNameCache = "";

	std::unordered_map<std::string, std::filesystem::path> bodyPaths;
	FileUtils::ListPaths("../Textures/CelestialBodies/", bodyPaths);

	ResourceCSVParser bodyCSVParser("../Data/CelestialBodyData.csv");

	// Process each CSV line and create a Body instance out of it
	for (const std::vector<std::string>& celestialBodyParams : bodyCSVParser.GetParsedCSV())
	{
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
			const float scaledTravelDistance = distanceToParent / sunEarthDistance * RADIUS_SCALE_FACTOR;
			scaledDistanceToParent = GetBodySystem(celestialBodyParentName).celestialBody.bodyData.radius + scaledTravelDistance;
		}
		// "Planet" and "Dwarf Planet" types
		else
		{
			const BodyData& celestialBodyData = GetBodySystem(celestialBodyNameCache).celestialBody.bodyData;
			const float scaledTravelDistance = distanceToParent / sunEarthDistance * DISTANCE_SCALE_FACTOR;
			if (celestialBodyName == "Mercury")
			{
				scaledDistanceToParent = celestialBodyData.radius * 2.0f + scaledTravelDistance;
			}
			else
			{
				scaledDistanceToParent = celestialBodyData.distanceToParent + scaledTravelDistance;
			}
		}

		const std::filesystem::path& texturePath = bodyPaths[celestialBodyName];
		const float scaledRadius = std::stof(celestialBodyParams[2]) / earthRadius * (celestialBodyType == "Star" ? 0.5f : 1.0f);
		const float obliquity = std::stof(celestialBodyParams[4]);
		const float scaledOrbitalPeriod = std::stof(celestialBodyParams[5]) * (celestialBodyType == "DwarfPlanet" ? GetBodySystem("Earth").celestialBody.bodyData.orbitalPeriod : 1.0f);
		const float spinPeriod = std::stof(celestialBodyParams[6]);
		const float orbitalInclination = std::stof(celestialBodyParams[7]);

		bodySystems.emplace_back(BodyData{ texturePath, celestialBodyName, scaledRadius, scaledDistanceToParent, obliquity, scaledOrbitalPeriod, spinPeriod, orbitalInclination, celestialBodyParentName });

		celestialBodyNameCache = celestialBodyName;
	}
}

void SolarSystem::BuildBodyRings()
{
	std::unordered_map<std::string, std::filesystem::path> ringPaths;
	FileUtils::ListPaths("../Models/Rings/", ringPaths);

	ResourceCSVParser ringCSVParser("../Data/RingData.csv");

	// Process each CSV line and create a Rings instance out of it
	for (const std::vector<std::string>& ringParams : ringCSVParser.GetParsedCSV())
	{
		const std::string& bodyName = ringParams[0];
		const std::filesystem::path& modelPath = ringPaths[ringParams[1]];
		const float radius = std::stof(ringParams[2]);
		const float opacity = std::stof(ringParams[3]);

		// Add the Rings to each parent Body once initialised
		BodySystem& parentBodySystem = GetBodySystem(bodyName);
		parentBodySystem.celestialBodyRings = std::make_unique<BodyRings>(RingsData{ modelPath, bodyName, radius, opacity  /*, &parentBodySystem.celestialBody*/ });
	}
}

void SolarSystem::BuildBelts()
{
	std::unordered_map<std::string, std::filesystem::path> beltPaths;
	FileUtils::ListPaths("../Models/Belts/", beltPaths);

	ResourceCSVParser beltCSVParser("../Data/BeltData.csv");

	// Process each CSV line and create a Belt instance out of it
	for (const std::vector<std::string>& beltParams : beltCSVParser.GetParsedCSV())
	{
		const std::string& beltName = beltParams[0];
		const std::filesystem::path& modelPath = beltPaths[beltParams[1]];
		const uint32_t instanceCount = std::stoi(beltParams[2]);
		const float sizeRangeLowerBound = std::stof(beltParams[3]);
		const uint32_t sizeRangeSpan = std::stoi(beltParams[4]);
		const float outerBound = GetBodySystem(beltParams[5]).celestialBody.bodyData.distanceToParent;
		const float innerBound = GetBodySystem(beltParams[6]).celestialBody.bodyData.distanceToParent;

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

		belts.emplace_back(
			beltName,
			InstanceParams{ modelPath, instanceCount, sizeRangeLowerBound, sizeRangeSpan },
			TorusParams{ majorRadius, minorRadius, flatnessFactor });
	}
}

BodySystem& SolarSystem::GetBodySystem(const std::string& inBodyName)
{
	const auto& bodyIt = std::find_if(bodySystems.begin(), bodySystems.end(), [&inBodyName](const BodySystem& body)
	{
		return body.celestialBody.GetName() == inBodyName;
	});

	if (bodyIt == bodySystems.end())
	{
		std::cout << "ERROR::SOLAR_SYSTEM - Celestial body " << inBodyName << " has not been found. Therefore, the iterator is pointing to nullptr, and accessing its content is provoking the crash." << std::endl;
	}

	return *bodyIt;
}