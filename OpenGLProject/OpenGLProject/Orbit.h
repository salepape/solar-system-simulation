#ifndef ORBIT_H
#define ORBIT_H

#include <glm/glm.hpp>					// for cos and sin
#include <glm/gtc/constants.hpp>		// for pi

#include <vector>

#include "VertexArray.h"
#include "Texture.h"
#include "Renderer.h"




// Approximation : all planet / moon orbits will be circular (eccentricities close to 0)
class Orbit
{
private:
	float radius;					// Radius of the orbit (= distance between sun and corresponding planet) 
	unsigned int nbMeridStrips;		// Number of edges (controls degree of smoothness)
	VertexArray * vao;

	Texture * texture;

	// Vectors containing sphere data computed
	std::vector<float> vertCoor;

	void Compute();
	void Store();

public:
	Orbit(const char * path, float radiusArg);
	~Orbit();

	void Render(Renderer renderer);
};

#endif ORBIT_H
