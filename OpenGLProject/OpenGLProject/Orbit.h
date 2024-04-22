#ifndef ORBIT_H
#define ORBIT_H

#include <iostream>
#include <string>

#include "Mesh.h"

class Renderer;



// Approximation: all planet/moon orbits will be circular (eccentricities close to 0)
class Orbit : public Mesh
{
public:
	Orbit(const std::string& texturePath, const float inRadius);

	void Render(const Renderer& renderer, const unsigned int textureUnit) const override;

protected:
	void ComputeVertices() override;

private:
	float radius{ 0.0f };
	int meridianStripsCount{ 100 };
};



#endif // ORBIT_H