#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <string>

#include "Mesh.h"



class Sphere: public Mesh
{
public:
	Sphere(const std::string& inTexturePath, const float inRadius, const uint32_t inMeridianStripsCount = 100, const uint32_t inParallelStripsCount = 100);

protected:
	// Note that we call square the zone created by the intersection between one meridian strip and one parallel strip
	void ComputeVertices() override;
	void ComputeIndices();

private:
	float radius{ 0.0f };
	uint32_t meridianStripsCount{ 0 };
	uint32_t parallelStripsCount{ 0 };
};



#endif // SPHERE_H