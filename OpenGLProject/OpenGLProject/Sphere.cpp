#include "Sphere.h"

#include <glad.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Texture.h"


Sphere::Sphere(const std::string& inTexturePath, const float inRadius, const int inMeridianStripsCount, const int inParallelStripsCount) : 
	radius(inRadius), meridianStripsCount(inMeridianStripsCount), parallelStripsCount(inParallelStripsCount)
{
	Texture texture(inTexturePath, GL_TEXTURE_2D, GeometryType::SPHERE, MapType::NONE);
	texture.LoadDDS();
	textures.push_back(texture);

	Compute();
	Store();
}

Sphere::~Sphere()
{

}

void Sphere::Compute()
{
	for (int i = 0; i <= parallelStripsCount; ++i)
	{
		const float pi = glm::pi<float>();

		// Angle between two squares of one parallel strip (in radians)
		const float theta = pi * (0.5f - static_cast<float>(i) / static_cast<float>(parallelStripsCount));
		const float rCosTheta = radius * glm::cos(theta);
		const float zCoor = radius * glm::sin(theta);

		for (int j = 0; j <= meridianStripsCount; ++j)
		{
			Vertex vertex;

			// Angle between two squares of one meridian strip (in radians)
			const float phi = 2.0f * pi * static_cast<float>(j) / static_cast<float>(meridianStripsCount);
			const float xCoor = rCosTheta * glm::cos(phi);
			const float yCoor = rCosTheta * glm::sin(phi);
			vertex.Position = glm::vec3(xCoor, yCoor, zCoor);

			const float invLength = 1.0f / radius;
			vertex.Normal = glm::vec3(xCoor * invLength, yCoor * invLength, zCoor * invLength);

			vertex.TexCoords = glm::vec2(static_cast<float>(j) / meridianStripsCount, static_cast<float>(i) / parallelStripsCount);

			vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
			vertex.Bitangent = glm::vec3(0.0f, 0.0f, 0.0f);

			vertices.push_back(vertex);
		}
	}

	// Indices
	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1
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