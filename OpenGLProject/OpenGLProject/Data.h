#include <array>
#include <glm/vec3.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Orbit.h"
#include "Sphere.h"
#include "Texture.h"

constexpr uint32_t BODIES_COUNT = 19;
constexpr float DIST_SCALE_FACTOR = 10.0f;



// Paths to retrieve DDS textures
std::array<const std::string, BODIES_COUNT> texturePaths
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
	"../Textures/Satellites/callisto.dds",
	"../Textures/Satellites/europa.dds",
	"../Textures/Satellites/ganymede.dds",
	"../Textures/Satellites/io.dds",
	"../Textures/Satellites/titan.dds",
	"../Textures/Satellites/triton.dds",
	//"../Textures/Satellites/deimos.dds",
	//"../Textures/Satellites/phobos.dds",

	// Normal maps
	"../Textures/Planets/8k_earth_normal_map.png"
};



// Represent all info relative to a celestial body
struct EntityInfo
{
	std::string texturePath;				// DDS texture path
	float radius{ 0.0f };					// Planet or moon (only those > pluto radius in length) radius divided by earth's radius [in kms]
	float dist{ 0.0f };						// Distance between planet (resp. moon) and sun (resp. the planet around which they gravitate) divided by sun-earth distance [in kms]
	float obliquity{ 0.0f };				// Or axial tilt: angle between planet/moon axis rotation and the normal of its orbital plane [in degrees]
	float orbPeriod{ 0.0f };				// Time the planet (resp. moon) takes to do 1 revolution around the sun (resp. its planet) [in earth days]
	float rotPeriod{ 0.0f };				// Time the planet takes to do a rotation on itself [in earth days]	
	float orbInclination{ 0.0f };			// Or "orbital tilt": angle between planet (resp. moon) orbit and the ecliptic [in degrees]
	Sphere* sphere{ nullptr };				// Pointer to the sphere mesh
	Orbit* orbit{ nullptr };				// Pointer to the orbit mesh
	std::string parentName;					// Name of the planet mesh around which a moon rotates
	EntityInfo* parentInfo{ nullptr };		// Pointer to the planet mesh around which a moon rotates
	float angleRot{ 0.0f };					// Filled in later: angle of planet rotation around the sun per frame [in radians]
};

// Solar System celestial bodies data
std::unordered_map<std::string, EntityInfo> data(BODIES_COUNT);

// Represent all constant formula parts (so we can compute them outside the Render loop)
struct PreComputations
{
	float angleRotCst{ 0.0f };
	float angleRotItselfCst{ 0.0f };
	float rotPeriodCst{ 0.0f };

	float orbInclinationInRad{ 0.0f };
	float cosCircularTslCst{ 0.0f };
	float sinCircularTslCst{ 0.0f };

	float obliquityInRad{ 0.0f };

	glm::vec3 nonSphericalScaling;

	float textHeight{ 0.0f };
	float sunTextHeight{ 0.0f };

	float textScale{ 0.0f };
	float sunTextScale{ 0.0f };
};

// Solar System celestial bodies pre-computations
std::unordered_map<std::string, PreComputations> preComputations(BODIES_COUNT);



// Fill data structures storing all celestial bodies information/pre-computations (collected from NASA website, wikipedia and passionates)
// Note: data is scaled so we can visualise the whole Solar System without having to travel too much time.
void LoadData()
{
	using dataElmt = std::pair<std::string, EntityInfo>;

	data.insert(dataElmt("Sun",			{ texturePaths[0], 109.3f, 0.0f, 7.25f, 0.0f, 27.0f }));

	data.insert(dataElmt("Mercury",		{ texturePaths[1],  0.383f, data["Sun"].radius + 0.38f * DIST_SCALE_FACTOR,      0.03f,  87.97f,    58.6f,   7.01f }));
	data.insert(dataElmt("Venus",		{ texturePaths[2],  0.95f,  data["Mercury"].dist + 0.72f * DIST_SCALE_FACTOR,    2.64f,  224.7f,   -243.02f, 3.39f }));
	data.insert(dataElmt("Earth",		{ texturePaths[3],  1.0f,	data["Venus"].dist + 1.0f * DIST_SCALE_FACTOR,	     23.44f, 365.26f,   0.99f,   0.0f  }));
	data.insert(dataElmt("Mars",		{ texturePaths[4],  0.532f, data["Earth"].dist + 1.52f * DIST_SCALE_FACTOR,	     25.19f, 686.97f,   1.03f,   1.85f }));
	data.insert(dataElmt("Jupiter",		{ texturePaths[5],  10.97f, data["Mars"].dist + 5.19f * DIST_SCALE_FACTOR,	     3.13f,  4332.59f,  0.41f,   1.31f }));
	data.insert(dataElmt("Saturn",		{ texturePaths[6],  9.14f,  data["Jupiter"].dist + 9.53f * DIST_SCALE_FACTOR,    26.73f, 10759.22f, 0.43f,   2.49f }));
	data.insert(dataElmt("Uranus",		{ texturePaths[7],  3.981f, data["Saturn"].dist + 19.20f * DIST_SCALE_FACTOR,    82.23f, 30688.5,  -0.72f,   0.77f }));
	data.insert(dataElmt("Neptune",		{ texturePaths[8],  3.865f, data["Uranus"].dist + 30.05f * DIST_SCALE_FACTOR,	 28.32f, 60182.0f,  0.67f,   1.77f }));

	data.insert(dataElmt("Ceres",		{ texturePaths[9],  0.074f, data["Mars"].dist + 2.75f * DIST_SCALE_FACTOR,       0.0f,   1683.15f,  9.07f,   17.14f }));
	data.insert(dataElmt("Pluto",		{ texturePaths[10], 0.186f, data["Neptune"].dist + 39.24f * DIST_SCALE_FACTOR,   57.47f, 90560.0f, -6.39f,   10.62f }));

	data.insert(dataElmt("Luna",		{ texturePaths[11], 0.273f, data["Earth"].radius + 0.384f * DIST_SCALE_FACTOR,   6.68f,  27.32f,    27.32f,  5.145f,	nullptr,  nullptr, "Earth",		&data["Earth"]   }));
	data.insert(dataElmt("Callisto",	{ texturePaths[12], 0.378f, data["Jupiter"].radius + 1.883f * DIST_SCALE_FACTOR, 0.0f,   16.69f,    16.69f,  2.017f,	nullptr,  nullptr, "Jupiter",	&data["Jupiter"] }));
	data.insert(dataElmt("Europa",		{ texturePaths[13], 0.245f, data["Jupiter"].radius + 0.671f * DIST_SCALE_FACTOR, 0.1f,   3.55f,     3.55f,   1.791f,	nullptr,  nullptr, "Jupiter",	&data["Jupiter"] }));
	data.insert(dataElmt("Ganymede",	{ texturePaths[14], 0.413f, data["Jupiter"].radius + 1.07f * DIST_SCALE_FACTOR,  0.16f,  7.15f,     7.15f,   2.214f,	nullptr,  nullptr, "Jupiter",	&data["Jupiter"] }));
	data.insert(dataElmt("Io",			{ texturePaths[15], 0.286f, data["Jupiter"].radius + 0.422f * DIST_SCALE_FACTOR, 0.0f,   1.77f,     1.77f,   2.213f,	nullptr,  nullptr, "Jupiter",	&data["Jupiter"] }));
	data.insert(dataElmt("Titan",		{ texturePaths[16], 0.404f, data["Saturn"].radius + 1.222f * DIST_SCALE_FACTOR,  0.0f,   15.95f,    15.95f,  0.0f,		nullptr,  nullptr, "Saturn",	&data["Saturn"]	 }));
	data.insert(dataElmt("Triton",		{ texturePaths[17], 0.212f, data["Neptune"].radius + 0.354f * DIST_SCALE_FACTOR, 0.0f,   5.88f,     5.88f,   129.812f,	nullptr,  nullptr, "Neptune",	&data["Neptune"] }));
	//data.insert(dataElmt("Deimos",		{ texturePaths[18], 0.000f,	data["Mars"].radius + 0.156f * DIST_SCALE_FACTOR,	 0.0f,   1.263f,    1.263f,  nullptr, nullptr, 27.58f,  "Mars", &data["Mars"])    }));
	//data.insert(dataElmt("Phobos",		{ texturePaths[19], 0.000f,	data["Mars"].radius + 0.94f * DIST_SCALE_FACTOR,	 0.0f,	 0.319f,    0.319f,  nullptr, nullptr, 28.4f,	"Mars", &data["Mars"])    }));
}

void LoadPreComputations()
{
	const float doublePi = 2.0f * glm::pi<float>();

	using preComputationsElmt = std::pair<std::string, PreComputations>;
	for (const auto& dataInput : data)
	{
		const float orbInclination = glm::radians(dataInput.second.orbInclination);
		const glm::vec3& nonSphericalScale = dataInput.second.sphere ? glm::vec3() : glm::vec3(0.025f * dataInput.second.parentInfo->radius);

		PreComputations preComp
		{
			doublePi * 1.0f / dataInput.second.orbPeriod,
			doublePi * dataInput.second.orbPeriod,
			doublePi * dataInput.second.rotPeriod,

			orbInclination,
			dataInput.second.dist * cos(orbInclination),
			dataInput.second.dist * sin(orbInclination),

			glm::radians(dataInput.second.obliquity),

			nonSphericalScale,

			dataInput.second.radius * 1.25f,
			dataInput.second.radius * 1.25f * 0.5f,

			dataInput.second.radius * 0.01f,
			dataInput.second.radius * 0.003f
		};

		preComputations.insert(preComputationsElmt(dataInput.first, preComp));
	}
}