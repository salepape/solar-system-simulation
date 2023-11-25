#ifndef ORBIT_H
#define ORBIT_H

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

#include "Mesh.h"



// Approximation : all planet / moon orbits will be circular (eccentricities close to 0)
class Orbit : public Mesh
{
private:
	// Radius of the orbit (= distance between sun and corresponding planet) 
	float radius{ 0.0f };

	// Number of edges (controls degree of smoothness)
	int nbMeridStrips = 100;

	Texture* texture{ nullptr };

protected:
	void Compute() override;

public:
	Orbit(const char* texturePath, const float radiusArg);
	~Orbit();

	void Render(const Renderer& renderer, const unsigned int& textureUnit) override;
};



#endif // ORBIT_H
