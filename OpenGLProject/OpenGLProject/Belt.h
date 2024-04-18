#ifndef BELT_H
#define BELT_H

#include <glm/mat4x4.hpp>
#include <iostream>
#include <vector>

#include "Model.h"
#include "VertexArray.h"

class Renderer;



// Instance is an asteroid for a belt
struct InstanceParams
{
	Model model;

	const unsigned int count{ 0 };

	// Range from where a random number will be picked from to determine the model size
	const float sizeRangeLowerBound{ 0.0f };
	const unsigned int sizeRangeSpan{ 0 };

	InstanceParams() {};
	InstanceParams(const Model& inModel, const unsigned int inCount, const float inSizeRangeLowerBound, const unsigned int inSizeRangeSpan) :
		model(inModel), count(inCount), sizeRangeLowerBound(inSizeRangeLowerBound), sizeRangeSpan(inSizeRangeSpan) {};
};

// Torus built with instances (not with vertices like a real geometrical shape)
struct TorusParams
{
	// Distance tube center - torus center
	const float majorRadius{ 0.0f };

	// Tube radius
	const float minorRadius{ 0.0f };

	// Factor reducing y-coordinate value for each instance
	const float flatnessFactor{ 0.0f };
};

class Belt
{
public:
	Belt();
	Belt(const InstanceParams& inInstanceParams, const TorusParams& inTorusParams);
	~Belt();

	void Render(const Renderer& renderer, const unsigned int& textureUnit);

private:
	InstanceParams instanceParams;
	TorusParams torusParams;
	std::vector<glm::mat4> modelMatrices;

	void ComputeModelMatrices();
	void StoreModelMatrices();

	static constexpr int INSTANCE_MATRIX_ELMTS_COUNT = 4;
};



#endif // BELT_H