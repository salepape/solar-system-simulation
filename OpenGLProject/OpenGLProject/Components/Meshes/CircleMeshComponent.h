#ifndef CIRCLE_H
#define CIRCLE_H

#include <cstdint>

#include "MeshComponent.h"



class CircleMeshComponent : public MeshComponent
{
public:
	CircleMeshComponent(const float inRadius);

	void Render() const override;

private:
	float radius{ 0.0f };
	uint32_t meridianStripsCount{ 500 };

	void ComputeVertices();
};



#endif // CIRCLE_H