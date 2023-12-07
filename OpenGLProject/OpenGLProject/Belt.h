#ifndef BELT_H
#define BELT_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include "Model.h"
#include "VertexArray.h"

class Renderer;



class Belt
{
private:
	unsigned int asteroidNb{ 0 };
	int sizeFactor{ 1 };
	float majorRadius{ 0.0f };
	float minorRadius{ 0.0f };
	Model asteroid;
	glm::mat4* modelMatrices;

	VertexArray vao;

	void Compute();
	void Store();

	static constexpr int GetInstanceMatrixNumElmts() { return 4; }

public:
	Belt(const Model asteroidArg, const unsigned int asteroidNbArg, const int sizeFactorArg, const float majorRadiusArg, const float minorRadiusArg);
	~Belt();

	void Render(const Renderer& renderer, const unsigned int& textureUnit);
};



#endif // BELT_H