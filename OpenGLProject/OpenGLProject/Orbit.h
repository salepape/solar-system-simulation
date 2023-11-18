#ifndef ORBIT_H
#define ORBIT_H

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"



// Approximation : all planet / moon orbits will be circular (eccentricities close to 0)
class Orbit
{
private:
	float radius;							// Radius of the orbit (= distance between sun and corresponding planet) 
	unsigned int nbMeridStrips = 100;		// Number of edges (controls degree of smoothness)
	std::vector<float> vertCoor;			// Vectors containing sphere data computed
	Texture* texture;

	VertexArray* vao;

	void Compute();
	void Store();

public:
	Orbit(const char* path, const float radiusArg);
	~Orbit();

	void Render(const Renderer& renderer, const unsigned int& textureUnit);
};



#endif // ORBIT_H
