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

	// To be called after instantiation of Body Systems, as some of their parameters are used to position the user at start
	void SetInitialTransform(const glm::vec3& inPosition, const glm::vec3& inRotation);

	PerspectiveCameraController& GetCameraController() { return cameraController; }

private:
	PerspectiveCameraController cameraController;

	void ComputeModelMatrixVUniform(const float /*deltaTime*/) override {};
};



#endif // SPACECRAFT_H
