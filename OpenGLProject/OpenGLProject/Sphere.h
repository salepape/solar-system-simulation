#ifndef SPHERE_H
#define SPHERE_H

#include <cstdint>

#include "MeshComponent.h"



class SphereMeshComponent : public MeshComponent
{
public:
	SphereMeshComponent(const float inRadius, const uint32_t inMeridianStripsCount = 100, const uint32_t inParallelStripsCount = 100);

	// Implicit copy constructor will be called when GetBodySystem() is called, as we get a shallow copy of it every time

private:
	float radius{ 0.0f };
	uint32_t meridianStripsCount{ 0 };
	uint32_t parallelStripsCount{ 0 };

	void ComputeVertices();
	void ComputeIndices();
};



#endif // SPHERE_H