#ifndef ORBIT_H
#define ORBIT_H

#include <glad/glad.h>

#include <glm/glm.hpp>					// for cos and sin
#include <glm/gtc/constants.hpp>		// for pi

#include <vector>

#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"



// Approximation : all planet / moon orbits will be circular (eccentricities close to 0)
class Orbit
{
private:
	float radius;				// Radius of the orbit (= distance between sun and corresponding planet) 
	int nbMeridStrips;			// Number of edges (controls degree of smoothness)
	VertexArray * vao;

	// Vectors containing sphere data computed
	std::vector<float> vertCoor;
	Texture * texture;

	void Compute();
	void Store();

public:
	Orbit(const char * texturePath, float radiusArg);
	~Orbit();

	void Draw();
};

#endif ORBIT_H
