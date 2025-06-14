#ifndef BELT_H
#define BELT_H

#include <filesystem>
#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

#include "Material.h"
#include "Model.h"
#include "SceneEntity.h"

class Renderer;



// Instance is an asteroid for a belt
struct InstanceParams
{
	std::filesystem::path modelPath;

	const uint32_t count{ 0 };

	// Range from where a random number will be picked from to determine the model size
	const float sizeRangeLowerBound{ 0.0f };
	const uint32_t sizeRangeSpan{ 0 };
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

class Belt : public SceneEntity
{
public:
	Belt(const std::string& inName, InstanceParams&& inInstanceParams, TorusParams&& inTorusParams);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	InstanceParams instanceParams;
	TorusParams torusParams;
	std::vector<glm::mat4> modelMatrices;

	// Model used for a belt body
	Model model;

	static Material InitialiseParent(const std::filesystem::path& inTexturePath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override {};
	void ComputeInstanceModelMatrices();
	void StoreInstanceModelMatrices();
};



#endif // BELT_H