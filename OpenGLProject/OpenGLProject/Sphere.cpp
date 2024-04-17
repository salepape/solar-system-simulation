#include "Sphere.h"

#include <glad.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Texture.h"



Sphere::Sphere(const std::string& inTexturePath, const float inRadius, const int inMeridianStripsCount, const int inParallelStripsCount) :
	radius(inRadius), meridianStripsCount(inMeridianStripsCount), parallelStripsCount(inParallelStripsCount),
	Mesh(inTexturePath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, TextureType::NONE)
{
	ComputeVertices();
	ComputeIndices();
	StoreVertices();
}

Sphere::~Sphere()
{

}

void Sphere::ComputeVertices()
{
	const float pi = glm::pi<float>();
	const float doublePi = 2.0f * pi;

	const float invMeridianStripsCount = 1.0f / static_cast<float>(meridianStripsCount);
	const float invParallelStripsCount = 1.0f / static_cast<float>(parallelStripsCount);
	const float invRadius = 1.0f / radius;

	const glm::vec3 zeroVector(0.0f, 0.0f, 0.0f);

	vertices.reserve((parallelStripsCount + 1) * (meridianStripsCount + 1));
	for (int i = 0; i <= parallelStripsCount; ++i)
	{
		const float iInvParallelStripsCount = static_cast<float>(i) * invParallelStripsCount;

		// Angle between two squares of one parallel strip (in radians)
		const float theta = pi * (0.5f - iInvParallelStripsCount);
		const float rCosTheta = radius * glm::cos(theta);
		const float zCoor = radius * glm::sin(theta);

		for (int j = 0; j <= meridianStripsCount; ++j)
		{
			// Angle between two squares of one meridian strip (in radians)
			const float jInvMeridianStripsCount = static_cast<float>(j) * invMeridianStripsCount;
			const float phi = doublePi * jInvMeridianStripsCount;
			const float xCoor = rCosTheta * glm::cos(phi);
			const float yCoor = rCosTheta * glm::sin(phi);

			Vertex vertex;
			vertex.Position = glm::vec3(xCoor, yCoor, zCoor);
			vertex.Normal = glm::vec3(xCoor * invRadius, yCoor * invRadius, zCoor * invRadius);
			vertex.TexCoords = glm::vec2(jInvMeridianStripsCount, iInvParallelStripsCount);
			vertex.Tangent = zeroVector;
			vertex.Bitangent = zeroVector;
			vertices.push_back(vertex);
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
	for (int i = 0; i < meridianStripsCount; ++i)
	{
		unsigned int parallelStripIndice = i * (parallelStripsCount + 1);
		unsigned int nextParallelStripIndice = parallelStripIndice + meridianStripsCount + 1;

		for (int j = 0; j < meridianStripsCount; ++j, ++parallelStripIndice, ++nextParallelStripIndice)
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