#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include "Model.h"



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

	void Draw();
};











