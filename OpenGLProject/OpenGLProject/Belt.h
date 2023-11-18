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
	Belt(const Model asteroidArg, const unsigned int asteroidNbArg, const int sizeFactorArg, const float majorRadiusArg, const float minorRadiusArg);
	~Belt();

	void Render(const Renderer& renderer, const unsigned int& textureUnit);
};



#endif // BELT_H









