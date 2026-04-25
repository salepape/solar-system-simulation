#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include <glm/vec3.hpp>

#include "Interactions/PerspectiveCameraController.h"
#include "SceneEntity.h"



// Representation of the Player in the Simulation as First-Person (i.e. no visual representation of the Spaceship to be rendered on screen)
class SpacecraftEntity : public SceneEntity
{
public:
	SpacecraftEntity(const glm::vec3& inPosition = glm::vec3(0.0f), const glm::vec3& inRotation = glm::vec3(0.0f), const float inZoomMaxLevel = 45.0f, const float inFarPlane = 100000000.0f);

	// Needed since all Body Systems are built past the initialisation-list of Solar System, and some values are needed to initialise position start
	void SetInitialTransform(const glm::vec3& inPosition, const glm::vec3& inRotation);

	PerspectiveCameraController& GetCameraController() { return cameraController; }

private:
	PerspectiveCameraController cameraController;

	void ComputeModelMatrixVUniform(const float /*elapsedTime*/) override {};
};



#endif // SPACECRAFT_H