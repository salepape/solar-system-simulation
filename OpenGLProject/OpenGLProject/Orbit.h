#ifndef ORBIT_H
#define ORBIT_H

#include <iostream>
#include <string>

#include "Mesh.h"

class Renderer;



// @todo - Implement elliptic orbits for a more realistic simulation (circular orbits with eccentricities close to 0 for now)
class Orbit : public Mesh
{
public:
	Orbit(const std::string& texturePath, const float inRadius);

	void Render(const Renderer& renderer, const uint32_t textureUnit) const override;

protected:
	void ComputeVertices() override;

private:
	float radius{ 0.0f };
	uint32_t meridianStripsCount{ 100 };
};



#endif // ORBIT_H