#ifndef BELT_H
#define BELT_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <vector>

#include "Model.h"
#include "VertexArray.h"

class Renderer;



class Belt
{
public:
	Belt(const Model inInstanceModel, const unsigned int inInstanceModelNb, const int inSizeFactor, const float inMajorRadius, const float inMinorRadius);
	~Belt();

	void Render(const Renderer& renderer, const unsigned int& textureUnit);

private:
	unsigned int instanceModelNb{ 0 };
	int sizeFactor{ 1 };
	float majorRadius{ 0.0f };
	float minorRadius{ 0.0f };
	Model instanceModel;
	std::vector<glm::mat4> modelMatrices;

	VertexArray vao;

	void Compute();

	// We could instantiate several belts with the same model (ie its VAO ID) if modelMatrices gathered data of all belts
	void Store();

	static constexpr int GetInstanceMatrixNumElmts() { return 4; }
};



#endif // BELT_H