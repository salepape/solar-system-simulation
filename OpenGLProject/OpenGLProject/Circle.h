#ifndef CIRCLE_H
#define CIRCLE_H

#include <string>

#include "Mesh.h"

class Renderer;



class Circle : public Mesh
{
public:
	Circle(const std::string& texturePath, const float inRadius);

	void Render(const Renderer& renderer, const uint32_t textureUnit) const override;

protected:
	void ComputeVertices() override;

private:
	float radius{ 0.0f };
	uint32_t meridianStripsCount{ 100 };
};



#endif // CIRCLE_H