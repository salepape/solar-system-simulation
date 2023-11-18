#ifndef BELT_H
#define BELT_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include "Model.h"
#include "Renderer.h"



class Belt
{
private:
	unsigned int asteroidNb;
	int sizeFactor;
	float majorRadius;
	float minorRadius;
	Model asteroid;
	glm::mat4 * modelMatrices;

	VertexArray vao;

	void Compute();
	void Store();

public:
	Belt(Model asteroidArg, unsigned int asteroidNbArg, int sizeFactorArg, float majorRadiusArg, float minorRadiusArg);
	~Belt();

	void Render(Renderer& renderer, unsigned int& textureUnit);
};



#endif // BELT_H









