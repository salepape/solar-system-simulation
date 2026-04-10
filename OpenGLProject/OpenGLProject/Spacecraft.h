#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include <glm/vec3.hpp>

#include "PerspectiveCameraController.h"



// Not considered a SceneEntity, since it doesn't need a Mesh or a Material attached to it (doesn't have any visual representation yet)
class Spacecraft
{
public:
	Spacecraft(const glm::vec3& inPosition = glm::vec3(0.0f), const glm::vec3& inRotation = glm::vec3(0.0f), const float inZoomMaxLevel = 45.0f, const float inFarPlane = 100000000.0f);

	// Needed since all Body Systems are built past the initialisation-list of Solar System, and some values are needed to initialise position start
	void SetInitialTransform(const glm::vec3& inPosition, const glm::vec3& inRotation);

	PerspectiveCameraController& GetCameraController() { return cameraController; }

private:
	PerspectiveCameraController cameraController;
};



#endif // SPACECRAFT_H