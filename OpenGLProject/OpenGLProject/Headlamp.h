#ifndef HEADLAMP_H
#define HEADLAMP_H

#include <cstdint>
#include <glm/vec3.hpp>

#include "Lights/SpotLightComponent.h"

class Camera;



// Cast a beam of light from the Spaceship position in the direction of player's gaze
class Headlamp
{
public:
	Headlamp(const glm::vec3& inPosition);

	void UpdateHeadlight(const Camera& camera);

	// Call corresponding GLFW callback functions
	void UpdateHeadlightState(double timeBeforeReleaseRegistered, int32_t action);

private:
	SpotLightComponent headlight;

	double headlightStartTime{ 0.0 };

	// Turn on/off reflection params rather than creating/deleting a heap-allocated SpotLight instance
	void SetHeadlightState(const bool isActive);
};



#endif // HEADLAMP_H