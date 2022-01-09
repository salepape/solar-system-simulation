#include <vector>
#include <unordered_map>

#include "Sphere.h"
#include "Orbit.h"
#include "Texture.h"



#define DIST_SCALE_FACTOR 10.0f
#define nbTextures 19
//#define nbTextures 21



// Paths to retrieve DDS textures
const char * texturePaths[nbTextures] =
{
	"../Textures/8k_sun.dds",

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

	// Normal maps
	"../Textures/Planets/8k_earth_normal_map.png",

	//// Other entities
	//"../Textures/8k_sun.dds",
	//"../Textures/8k_sun.dds"
};



// Represent all info relative to a celestial body
struct EntityInfo
{
	const char * texturePath;	// DDS texture path
	float radius;				// Planets and moons (only those > pluto radius in length) radii divided by earth's radius value [in kms]
	float dist;					// Distances between planets (resp. moons) and sun (resp. the planet around which they turn) divided by earth's distance value [in kms]
	float obliquity;			// Or axial tilt : angle between planet / moon axis rotation and the normal of its orbital plane [in degrees]
	float orbPeriod;			// Time the planet (resp. moon) takes to do 1 revolution around the sun (resp. its planet) [in earth days]
	float rotPeriod;			// Time the planet takes to do a rotation on itself [in earth days]
	Sphere * sphere;			// Sphere mesh
	float orbInclination;		// Or "orbital tilt" : angle between planet / moon orbit and the ecliptic [in degrees]
	Orbit * orbit;				// Orbit mesh
	EntityInfo * planet;		// Reference to the planet object to be able to make computation for its moon
	float angleRot;				// Angle of planet rotation around the sun per frame [in radians]
};

// Solar System celestial bodies data
std::unordered_map<std::string, EntityInfo> data;



// Create data structure storing all celestial bodies information (collected from NASA website, wikipedia and passionates)
void LoadData()
{
	using dataElmt = std::pair<std::string, EntityInfo>;

	data.insert(dataElmt("Sun",			{ texturePaths[0], 109.3f, 0.0f, 7.25f, 0.0f, 27.0f, nullptr, 0.0f, nullptr, nullptr, 0.0f }));

	data.insert(dataElmt("Mercury",		{ texturePaths[1],  0.383f, data["Sun"].radius + 0.38f * DIST_SCALE_FACTOR,      0.03f,  87.97f,    58.6f,   nullptr, 7.01f,    nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Venus",		{ texturePaths[2],  0.95f,  data["Mercury"].dist + 0.72f * DIST_SCALE_FACTOR,    2.64f,  224.7f,   -243.02f, nullptr, 3.39f,    nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Earth",		{ texturePaths[3],  1.0f,	data["Venus"].dist + 1.0f * DIST_SCALE_FACTOR,	     23.44f, 365.26f,   0.99f,   nullptr, 0.0f,     nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Mars",		{ texturePaths[4],  0.532f, data["Earth"].dist + 1.52f * DIST_SCALE_FACTOR,	     25.19f, 686.97f,   1.03f,   nullptr, 1.85f,    nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Jupiter",		{ texturePaths[5],  10.97f, data["Mars"].dist + 5.19f * DIST_SCALE_FACTOR,	     3.13f,  4332.59f,  0.41f,   nullptr, 1.31f,    nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Saturn",		{ texturePaths[6],  9.14f,  data["Jupiter"].dist + 9.53f * DIST_SCALE_FACTOR,    26.73f, 10759.22f, 0.43f,   nullptr, 2.49f,    nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Uranus",		{ texturePaths[7],  3.981f, data["Saturn"].dist + 19.20f * DIST_SCALE_FACTOR,    82.23f, 30688.5,  -0.72f,   nullptr, 0.77f,    nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Neptune",		{ texturePaths[8],  3.865f, data["Uranus"].dist + 30.05f * DIST_SCALE_FACTOR,	 28.32f, 60182.0f,  0.67f,   nullptr, 1.77f,    nullptr, nullptr, 0.0f }));

	data.insert(dataElmt("Ceres",		{ texturePaths[9],  0.074f, data["Mars"].dist + 2.75f * DIST_SCALE_FACTOR,       0.0f,   1683.15f,  9.07f,   nullptr, 17.14f,   nullptr, nullptr, 0.0f }));
	data.insert(dataElmt("Pluto",		{ texturePaths[10], 0.186f, data["Neptune"].dist + 39.24f * DIST_SCALE_FACTOR,   57.47f, 90560.0f, -6.39f,   nullptr, 10.62f,   nullptr, nullptr, 0.0f }));

	data.insert(dataElmt("Luna",		{ texturePaths[11], 0.273f, data["Earth"].radius + 0.384f * DIST_SCALE_FACTOR,   6.68f,  27.32f,    27.32f,  nullptr, 5.145f,   nullptr, &data["Earth"] }));
	data.insert(dataElmt("Callisto",	{ texturePaths[12], 0.378f, data["Jupiter"].radius + 1.883f * DIST_SCALE_FACTOR, 0.0f,   16.69f,    16.69f,  nullptr, 2.017f,   nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Europa",		{ texturePaths[13], 0.245f, data["Jupiter"].radius + 0.671f * DIST_SCALE_FACTOR, 0.1f,   3.55f,     3.55f,   nullptr, 1.791f,   nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Ganymede",	{ texturePaths[14], 0.413f, data["Jupiter"].radius + 1.07f * DIST_SCALE_FACTOR,  0.16f,  7.15f,     7.15f,   nullptr, 2.214f,   nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Io",			{ texturePaths[15], 0.286f, data["Jupiter"].radius + 0.422f * DIST_SCALE_FACTOR, 0.0f,   1.77f,     1.77f,   nullptr, 2.213f,   nullptr, &data["Jupiter"] }));
	data.insert(dataElmt("Titan",		{ texturePaths[16], 0.404f, data["Saturn"].radius + 1.222f * DIST_SCALE_FACTOR,  0.0f,   15.95f,    15.95f,  nullptr, 0.0f,	  nullptr, &data["Saturn"] }));
	data.insert(dataElmt("Triton",		{ texturePaths[17], 0.212f, data["Neptune"].radius + 0.354f * DIST_SCALE_FACTOR, 0.0f,   5.88f,     5.88f,   nullptr, 129.812f, nullptr, &data["Neptune"] }));
}