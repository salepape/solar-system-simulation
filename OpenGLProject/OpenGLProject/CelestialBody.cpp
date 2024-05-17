#include "CelestialBody.h"

#include <glm/trigonometric.hpp>

#include "Utils.h"

uint32_t CelestialBody::bodyIDCounter = 0;



CelestialBody::CelestialBody(const std::string& inTexturePath, const float inRadius, const float inDistanceToParent, const float inObliquity, const float inOrbitalPeriod, const float inSpinPeriod, const float inOrbitalInclination, const int32_t inParentID) :
	texturePath(inTexturePath), radius(inRadius), distanceToParent(inDistanceToParent), obliquity(inObliquity), orbitalPeriod(inOrbitalPeriod), spinPeriod(inSpinPeriod), orbitalInclination(inOrbitalInclination), parentID(inParentID),
	sphere({ inTexturePath, inRadius }), orbit({ inTexturePath, inDistanceToParent }), preComputations(LoadPreComputations())
{
	GetNameFromTexturePath();

	bodyID = CelestialBody::bodyIDCounter++;
}

const PreComputations CelestialBody::LoadPreComputations()
{
	const float orbitalInclinationInRad = glm::radians(orbitalInclination);

	float textHeightFactor = 1.5f;
	float textScaleFactor = 0.01f;
	if (bodyName == "Sun")
	{
		textHeightFactor = 1.25f;
		textScaleFactor = 0.0075f;
	}
	// If the current celestial body is a satellite (i.e. has a parent)
	else if (parentID != -1)
	{
		textHeightFactor = 3.5f;
		textScaleFactor = 0.03f;
	}

	return {
		orbitalPeriod == 0.0f ? 0.0f : Utils::doublePi * 1.0f / orbitalPeriod,
		spinPeriod == 0.0f ? 0.0f : Utils::doublePi * 1.0f / spinPeriod,

		glm::radians(obliquity),
		orbitalInclinationInRad,

		distanceToParent * glm::cos(orbitalInclinationInRad),
		distanceToParent * glm::sin(orbitalInclinationInRad),

		radius * textHeightFactor,
		radius * textScaleFactor
	};
}

void CelestialBody::GetNameFromTexturePath()
{
	bodyName.reserve(texturePath.size());

	std::string::iterator it = texturePath.begin();
	for (it; *it != '_'; ++it);

	++it;
	for (it; *it != '_' && *it != '.'; ++it)
	{
		bodyName.push_back(*it);
	}

	bodyName[0] = toupper(bodyName[0]);
}