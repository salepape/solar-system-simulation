#include "Sphere.h"

#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <utility>

#include "Utils.h"



Sphere::Sphere(const float inRadius, const uint32_t inMeridianStripsCount, const uint32_t inParallelStripsCount) :
	radius(inRadius), meridianStripsCount(inMeridianStripsCount), parallelStripsCount(inParallelStripsCount)
{
	ComputeVertices();
	ComputeIndices();
	StoreVertices();
}

void Sphere::ComputeVertices()
{
	const float invMeridianStripsCount = 1.0f / meridianStripsCount;
	const float invParallelStripsCount = 1.0f / parallelStripsCount;
	const float invRadius = 1.0f / radius;

	const glm::vec3 zeroVector(0.0f);

	vertices.reserve((parallelStripsCount + 1) * (meridianStripsCount + 1));
	for (uint32_t i = 0; i <= parallelStripsCount; ++i)
	{
		const float iInvParallelStripsCount = i * invParallelStripsCount;

		// Angle between two squares of one parallel strip (in radians)
		const float theta = Utils::unitPi * (0.5f - iInvParallelStripsCount);
		const float rCosTheta = radius * glm::cos(theta);
		const float zCoor = radius * glm::sin(theta);

		for (uint32_t j = 0; j <= meridianStripsCount; ++j)
		{
			// Angle between two squares of one meridian strip (in radians)
			const float jInvMeridianStripsCount = j * invMeridianStripsCount;
			const float phi = Utils::doublePi * jInvMeridianStripsCount;
			const float xCoor = rCosTheta * glm::cos(phi);
			const float yCoor = rCosTheta * glm::sin(phi);

			Vertex vertex;
			vertex.position = glm::vec3(xCoor, yCoor, zCoor);
			vertex.normal = glm::vec3(xCoor * invRadius, yCoor * invRadius, zCoor * invRadius);
			vertex.texCoords = glm::vec2(jInvMeridianStripsCount, iInvParallelStripsCount);
			vertex.tangent = zeroVector;
			vertex.biTangent = zeroVector;
			vertices.push_back(std::move(vertex));
		}
	}
}

void Sphere::ComputeIndices()
{
	// Indices matrix shape
	// k1--k1+1
	// |  / |
	// | /  |
	// k2--k2+1

	indices.reserve(meridianStripsCount * meridianStripsCount);
	for (uint32_t i = 0; i < meridianStripsCount; ++i)
	{
		uint32_t parallelStripIndice = i * (parallelStripsCount + 1);
		uint32_t nextParallelStripIndice = parallelStripIndice + meridianStripsCount + 1;

		for (uint32_t j = 0; j < meridianStripsCount; ++j, ++parallelStripIndice, ++nextParallelStripIndice)
		{
			// 2 triangles per square except for those formed by first (top) and last (bottom) parallel strips
			if (i != 0)
			{
				// k1---k2---k1+1
				indices.push_back(parallelStripIndice);
				indices.push_back(nextParallelStripIndice);
				indices.push_back(parallelStripIndice + 1);
			}

			if (i != (parallelStripsCount - 1))
			{
				// k1+1---k2---k2+1
				indices.push_back(parallelStripIndice + 1);
				indices.push_back(nextParallelStripIndice);
				indices.push_back(nextParallelStripIndice + 1);
			}
		}
	}
}