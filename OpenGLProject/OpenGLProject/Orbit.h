#ifndef ORBIT_H
#define ORBIT_H

#include <glad/glad.h>

#include <glm/glm.hpp>					// for cos and sin
#include <glm/gtc/constants.hpp>		// for pi

#include <vector>



// Approximation : all planet / moon orbits will be circular (eccentricities close to 0)
class Orbit
{
private:
	// Number of edges (controls degree of smoothness)
	int nbMeridStrips;
	// Vertex Array Object
	unsigned int VAO;
	// Vertex Buffer Object
	unsigned int VBO;
	// Element Buffer Object
	unsigned int EBO;
	// Radius of the orbit (= distance between sun and corresponding planet) [in kms]
	float radius;



public:
	Orbit(float radiusArg);

	void Draw();

	~Orbit();
};

#endif ORBIT_H
