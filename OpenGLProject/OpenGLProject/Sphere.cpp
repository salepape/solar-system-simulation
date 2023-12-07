#include "Sphere.h"

#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Texture.h"


Sphere::Sphere(std::string texturePath, const float radiusArg) : radius(radiusArg)
{
	Texture* texture = new Texture(texturePath, GL_TEXTURE_2D, GeometryType::SPHERE, MapType::NONE);
	texture->LoadDDS();
	textures.push_back(*texture);

	Compute();
	Store();
}

Sphere::~Sphere()
{

}

void Sphere::Compute()
{
	constexpr int numParallelStrips = 100;
	constexpr int numMeridianStrips = 100;

	for (int i = 0; i <= numParallelStrips; ++i)
	{
		const float pi = glm::pi<float>();

		// Angle between two squares of one parallel strip (in radians)
		const float theta = pi * (0.5f - static_cast<float>(i) / static_cast<float>(numParallelStrips));
		const float rCosTheta = radius * glm::cos(theta);
		const float zCoor = radius * glm::sin(theta);

		for (int j = 0; j <= numMeridianStrips; ++j)
		{
			Vertex vertex;

			// Angle between two squares of one meridian strip (in radians)
			const float phi = 2.0f * pi * static_cast<float>(j) / static_cast<float>(numMeridianStrips);
			const float xCoor = rCosTheta * glm::cos(phi);
			const float yCoor = rCosTheta * glm::sin(phi);
			vertex.Position = glm::vec3(xCoor, yCoor, zCoor);

			const float invLength = 1.0f / radius;
			vertex.Normal = glm::vec3(xCoor * invLength, yCoor * invLength, zCoor * invLength);

			vertex.TexCoords = glm::vec2(static_cast<float>(j) / numMeridianStrips, static_cast<float>(i) / numParallelStrips);

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
	for (int i = 0; i < numParallelStrips; ++i)
	{
		unsigned int ParallelStripIndice = i * (numMeridianStrips + 1);
		unsigned int NextParallelStripIndice = ParallelStripIndice + numMeridianStrips + 1;

		for (int j = 0; j < numMeridianStrips; ++j, ++ParallelStripIndice, ++NextParallelStripIndice)
		{
			// 2 triangles per square except for those formed by first (top) and last (bottom) parallel strips
			if (i != 0)
			{
				// k1---k2---k1+1
				indices.push_back(ParallelStripIndice);
				indices.push_back(NextParallelStripIndice);
				indices.push_back(ParallelStripIndice + 1);
			}

			if (i != (numParallelStrips - 1))
			{
				// k1+1---k2---k2+1
				indices.push_back(ParallelStripIndice + 1);
				indices.push_back(NextParallelStripIndice);
				indices.push_back(NextParallelStripIndice + 1);
			}
		}
	}
}