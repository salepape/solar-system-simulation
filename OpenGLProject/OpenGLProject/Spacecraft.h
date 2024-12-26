#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include <glm/vec3.hpp>

#include "Controller.h"



class Spacecraft
{
public:
	Spacecraft(const glm::vec3& inPosition, const glm::vec3& inRotation, const float inZoomMaxLevel, const float inFarPlane);

	Controller cameraController;
};



#endif // SPACECRAFT_H