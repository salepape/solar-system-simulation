#ifndef CIRCLE_H
#define CIRCLE_H

#include <cstdint>

#include "MeshComponent.h"

class Renderer;



class Circle : public MeshComponent
{
public:
	Circle(const float inRadius);

	void Render(const Renderer& renderer) const override;

private:
	float radius{ 0.0f };
	uint32_t meridianStripsCount{ 500 };

	void ComputeVertices();
};



#endif // CIRCLE_H