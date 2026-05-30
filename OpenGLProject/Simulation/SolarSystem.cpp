#include "SolarSystem.h"

#include <glm/vec3.hpp>

#include <cstdint>
#include <unordered_map>

#include "Application/Application.h"
#include "Application/Window.h"
#include "Cameras/PerspectiveCamera.h"
#include "Entities/BeltEntity.h"
#include "Entities/BillboardEntity.h"
#include "Entities/BodyRingsEntity.h"
#include "Entities/CelestialBodyEntity.h"
#include "Entities/MilkyWayEntity.h"
#include "Entities/OrbitEntity.h"
#include "Utils/Helpers.h"



SolarSystem::SolarSystem()
{
	BuildBackground();
	BuildBodySystems();
	BuildBelts();

	Scene::SetSceneViewerTransformStart(
		glm::vec3(0.0f, GetBody("Sun").GetBodyData().radius * 1.75f, -25.0f),
		glm::vec3(0.0f, -25.0f, 90.0f));
}

// @todo - Should all be migrated to Scene class
void SolarSystem::Update(const float deltaTime, const std::optional<SceneEntityHandle> /*handle*/)
{
	const Application& runningApp = Application::GetInstance();
	const Window& runningWindow = runningApp.GetWindow();

	sceneViewer.ProcessUserInput(deltaTime);

	PerspectiveCamera& camera = sceneViewer.GetCamera();

	////////// 1 - DRAW OPAQUE ENTITIES //////////

	// @todo - To be store as command in render queue?
	camera.SetProjectionViewVUniform(ViewMode::InfiniteLookAt, runningWindow.GetAspectRatio());

	// MILKY WAY (not part of Scene Entities hash map, member variable of SS class)	
	Scene::Update(deltaTime, SceneEntityHandle::BACKGROUND);

	// @todo - To be store as command in render queue?
	camera.SetProjectionViewVUniform(ViewMode::FiniteLookAt, runningWindow.GetAspectRatio());
	camera.SetPositionFUniform();

	// CELESTIAL BODIES, ORBITS, BILLBOARDS & BELTS (part of Scene Entities hash map)
	Scene::Update(deltaTime, SceneEntityHandle::OPAQUE_ENTITY);

	////////// 2 - DRAW ENTITIES WITH LEVEL OF TRANSPARENCY //////////

	// ORDER PER DIST TO CAM
	Scene::OrderForTransparencyPass(camera.GetPosition());

	// BODY RINGS (part of Scene Entities hash map)
	Scene::Update(deltaTime, SceneEntityHandle::TRANSPARENT_ENTITY);
}

void SolarSystem::BuildBackground()
{
	// Background which can never be reached (based off a Skybox)
	Scene::AddEntity(
		SceneEntityHandle::BACKGROUND,
		std::make_unique<MilkyWayEntity>()
	);
}

void SolarSystem::BuildBodySystems()
{
	// Required to scale distance of current celestial body according to the previous one 
	// (diverging from proper simulation here, for travel end-user convenience)
	std::string celestialBodyNameCache("");

	const std::string currentSolutionPath(FileHelper::GetSolutionAbsolutePath());

	std::unordered_map<std::string, std::filesystem::path> bodyPaths;
	FileHelper::ListModelPaths(currentSolutionPath + "/Textures/CelestialBodies/", bodyPaths);

	ResourceCSVParser bodyCSVParser(currentSolutionPath + "/Data/CelestialBodyData.csv");
	Scene::AllocateMemory(bodyCSVParser.GetCSVLinesCount());

	// Required to scale radius and distance to Sun of each celestial body for end user experience convenience
	const std::vector<std::string>& EarthLine = bodyCSVParser.GetParsedCSVLine("Earth");
	const float earthRadius = std::stof(EarthLine[2]);
	const float sunEarthDistance = std::stof(EarthLine[3]);

	// Process each CSV line and create a Body instance out of it
	for (const std::vector<std::string>& celestialBodyParams : bodyCSVParser.GetParsedCSV())
	{
		const std::string celestialBodyName(celestialBodyParams[0]);
		const std::string celestialBodyType(celestialBodyParams[1]);
		const std::string celestialBodyParentName(celestialBodyType == "Moon" ? celestialBodyParams[8] : "");

		const float distanceToParent = std::stof(celestialBodyParams[3]);
		float scaledDistanceToParent = 0.0f;
		if (celestialBodyType == "Star")
		{
			scaledDistanceToParent = 0.0f;
		}
		else if (celestialBodyType == "Moon")
		{
			constexpr float earthRadiusScaleFactor = 1000.0f;

			const float scaledTravelDistance = distanceToParent / sunEarthDistance * earthRadiusScaleFactor;
			scaledDistanceToParent = GetBody(celestialBodyParentName).GetBodyData().radius + scaledTravelDistance;
		}
		// "Planet" and "Dwarf Planet" types
		else
		{
			constexpr float sunEarthDistanceScaleFactor = 10.0f;

			const BodyData& celestialBodyData = GetBody(celestialBodyNameCache).GetBodyData();
			const float scaledTravelDistance = distanceToParent / sunEarthDistance * sunEarthDistanceScaleFactor;
			if (celestialBodyName == "Mercury")
			{
				scaledDistanceToParent = celestialBodyData.radius * 2.0f + scaledTravelDistance;
			}
			else
			{
				scaledDistanceToParent = celestialBodyData.distanceToParent + scaledTravelDistance;
			}
		}

		const std::filesystem::path& texturePath(bodyPaths[celestialBodyName]);
		const float scaledRadius = std::stof(celestialBodyParams[2]) / earthRadius * (celestialBodyType == "Star" ? 0.5f : 1.0f);
		const float obliquity = std::stof(celestialBodyParams[4]);
		const float scaledOrbitalPeriod = std::stof(celestialBodyParams[5]) * (celestialBodyType == "DwarfPlanet" ? GetBody("Earth").GetBodyData().orbitalPeriod : 1.0f);
		const float spinPeriod = std::stof(celestialBodyParams[6]);
		const float orbitalInclination = std::stof(celestialBodyParams[7]);

		const BodyData bodyData({ texturePath, celestialBodyName, scaledRadius, scaledDistanceToParent, obliquity, scaledOrbitalPeriod, spinPeriod, orbitalInclination });

		const uint32_t addedBodyID = Scene::AddEntity(
			SceneEntityHandle::OPAQUE_ENTITY,
			std::make_unique<CelestialBodyEntity>(bodyData)
		);

		const bool isEntityMoonRelated = celestialBodyParentName.length() != 0;

		// Make Moon Transform the one of the parent Planet, not the Sun!
		if (isEntityMoonRelated)
		{
			Scene::TagEntityAsAttached(Scene::GetEntity(celestialBodyParentName)->GetID(), addedBodyID);
		}

		const uint32_t addedOrbitID = Scene::AddEntity(
			SceneEntityHandle::TRANSPARENT_ENTITY,
			std::make_unique<OrbitEntity>(bodyData)
		);

		// Make Moon Orbit Transform the one of the parent Planet, not the Moon itself!
		if (isEntityMoonRelated)
		{
			Scene::TagEntityAsAttached(Scene::GetEntity(celestialBodyParentName)->GetID(), addedOrbitID);
		}

		const uint32_t addedBillboardID = Scene::AddEntity(
			SceneEntityHandle::TRANSPARENT_ENTITY,
			std::make_unique<BillboardEntity>(bodyData)
		);

		// Make Billboard Transform the one of the planet/moon
		Scene::TagEntityAsAttached(addedBodyID, addedBillboardID);

		celestialBodyNameCache = celestialBodyName;
	}

	BuildBodyRings();
}

void SolarSystem::BuildBodyRings()
{
	const std::string currentSolutionPath(FileHelper::GetSolutionAbsolutePath());

	std::unordered_map<std::string, std::filesystem::path> ringPaths;
	FileHelper::ListModelPaths(currentSolutionPath + "/Models/Rings/", ringPaths);

	ResourceCSVParser ringCSVParser(currentSolutionPath + "/Data/RingData.csv");
	Scene::AllocateMemory(ringCSVParser.GetCSVLinesCount());

	// Process each CSV line and create a Rings instance out of it
	for (const std::vector<std::string>& ringParams : ringCSVParser.GetParsedCSV())
	{
		const std::string bodyParent(ringParams[0]);
		const std::filesystem::path modelPath(ringPaths[ringParams[1]]);
		const float radius = std::stof(ringParams[2]);

		// Create Rings Scene Entity and store it as transparent in IRenderable map, NOT in Body System
		const uint32_t addedBodyRingsID = Scene::AddEntity(
			SceneEntityHandle::TRANSPARENT_ENTITY,
			std::make_unique<BodyRingsEntity>(RingsData{ modelPath, bodyParent, radius })
		);

		Scene::TagEntityAsAttached(Scene::GetConstEntity(bodyParent)->GetID(), addedBodyRingsID);
	}
}

void SolarSystem::BuildBelts()
{
	const std::string currentSolutionPath(FileHelper::GetSolutionAbsolutePath());

	std::unordered_map<std::string, std::filesystem::path> beltPaths;
	FileHelper::ListModelPaths(currentSolutionPath + "/Models/Belts/", beltPaths);

	ResourceCSVParser beltCSVParser(currentSolutionPath + "/Data/BeltData.csv");
	Scene::AllocateMemory(beltCSVParser.GetCSVLinesCount());

	// Process each CSV line and create a Belt instance out of it
	for (const std::vector<std::string>& beltParams : beltCSVParser.GetParsedCSV())
	{
		const std::string beltName(beltParams[0]);
		const std::filesystem::path modelPath(beltPaths[beltParams[1]]);
		const uint32_t instanceCount = std::stoi(beltParams[2]);
		const float sizeRangeLowerBound = std::stof(beltParams[3]);
		const uint32_t sizeRangeSpan = std::stoi(beltParams[4]);
		const float outerBound = GetBody(beltParams[5]).GetBodyData().distanceToParent;
		const float innerBound = GetBody(beltParams[6]).GetBodyData().distanceToParent;

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

		Scene::AddEntity(SceneEntityHandle::OPAQUE_ENTITY,
			std::make_unique<BeltEntity>(
				beltName,
				InstanceParams{ modelPath, instanceCount, sizeRangeLowerBound, sizeRangeSpan },
				TorusParams{ majorRadius, minorRadius, flatnessFactor }
			)
		);
	}
}

CelestialBodyEntity& SolarSystem::GetBody(const std::string& bodyName) const
{
	// Warning: assumes body pointer is valid!
	return *dynamic_cast<CelestialBodyEntity*>(Scene::GetEntity(bodyName));
}
