#include "SphereMeshComponent.h"

#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <utility>

#include "Utils/Constants.h"



SphereMeshComponent::SphereMeshComponent(const float inRadius, const uint32_t inMeridianStripCount, const uint32_t inParallelStripCount) :
	radius(inRadius), meridianStripCount(inMeridianStripCount), parallelStripCount(inParallelStripCount)
{
	ComputeVertices();
	ComputeIndices();
	StoreVertices();
}

void SphereMeshComponent::ComputeVertices()
{
	const float invMeridianStripCount = 1.0f / meridianStripCount;
	const float invParallelStripCount = 1.0f / parallelStripCount;
	const float invRadius = 1.0f / radius;

	vertices.reserve((parallelStripCount + 1) * (meridianStripCount + 1));
	for (uint32_t i = 0; i <= parallelStripCount; ++i)
	{
		const float iInvParallelStripsCount = i * invParallelStripCount;

		// Angle between two squares (i.e. area formed by the intersection between one meridian strip and one parallel strip) of one parallel strip (in radians)
		const float theta = GLMConstants::unitPi * (0.5f - iInvParallelStripsCount);
		const float rCosTheta = radius * glm::cos(theta);
		const float zCoor = radius * glm::sin(theta);

		for (uint32_t j = 0; j <= meridianStripCount; ++j)
		{
			// Angle between two squares of one meridian strip (in radians)
			const float jInvMeridianStripsCount = j * invMeridianStripCount;
			const float phi = GLMConstants::doublePi * jInvMeridianStripsCount;
			const float xCoor = rCosTheta * glm::cos(phi);
			const float yCoor = rCosTheta * glm::sin(phi);

			Vertex vertex;
			vertex.position = glm::vec3(xCoor, yCoor, zCoor);
			vertex.normal = glm::vec3(xCoor * invRadius, yCoor * invRadius, zCoor * invRadius);
			vertex.texCoords = glm::vec2(jInvMeridianStripsCount, iInvParallelStripsCount);
			vertex.tangent = GLMConstants::zeroVector;
			vertex.biTangent = GLMConstants::zeroVector;
			vertices.push_back(std::move(vertex));
		}
	}
}

void SphereMeshComponent::ComputeIndices()
{
	// Indices matrix shape
	// k1--k1+1
	// |  / |
	// | /  |
	// k2--k2+1

	indices.reserve(meridianStripCount * meridianStripCount);
	for (uint32_t i = 0; i < meridianStripCount; ++i)
	{
		uint32_t parallelStripIndice = i * (parallelStripCount + 1);
		uint32_t nextParallelStripIndice = parallelStripIndice + meridianStripCount + 1;

		for (uint32_t j = 0; j < meridianStripCount; ++j, ++parallelStripIndice, ++nextParallelStripIndice)
		{
			// 2 triangles per square (i.e. area formed by the intersection between one meridian strip and one parallel strip) except for those formed by first (top) and last (bottom) parallel strips
			if (i != 0)
			{
				// k1---k2---k1+1
				indices.push_back(parallelStripIndice);
				indices.push_back(nextParallelStripIndice);
				indices.push_back(parallelStripIndice + 1);
			}

			if (i != (parallelStripCount - 1))
			{
				// k1+1---k2---k2+1
				indices.push_back(parallelStripIndice + 1);
				indices.push_back(nextParallelStripIndice);
				indices.push_back(nextParallelStripIndice + 1);
			}
		}
	}
}
