#ifndef BELT_H
#define BELT_H

#include <glm/mat4x4.hpp>

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "Models/Model.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "SceneEntity.h"



// Instance is an asteroid for a belt
struct InstanceParams
{
	std::filesystem::path modelPath;

	uint32_t count{ 0 };

	// Range from where a random number will be picked from to determine the model size
	float sizeRangeLowerBound{ 0.0f };
	uint32_t sizeRangeSpan{ 0 };
};

// Torus built with instances (not with vertices like a real geometrical shape)
struct TorusParams
{
	// Distance tube center - torus center
	float majorRadius{ 0.0f };

	// Tube radius
	float minorRadius{ 0.0f };

	// Factor reducing y-coordinate value for each instance
	float flatnessFactor{ 0.0f };
};

class BeltEntity : public SceneEntity
{
public:
	BeltEntity(const std::string& inName, InstanceParams&& inInstanceParams, TorusParams&& inTorusParams);

	void Render(const float elapsedTime = 0.0f) override;

private:
	InstanceParams instanceParams;
	TorusParams torusParams;
	std::vector<glm::mat4> modelMatrices;

	// Model used to represent a Belt "Rock" for instancing (contains the Mesh + the Material definition, as opposed to traditional SceneEntities)
	Model model;

	void ComputeModelMatrixVUniform(const float /*elapsedTime*/) override {};
	void ComputeInstanceModelMatrices();
	void StoreInstanceModelMatrices() const;
};



#endif // BELT_H