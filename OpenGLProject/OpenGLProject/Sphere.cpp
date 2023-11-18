#include "Sphere.h"



Sphere::Sphere(const char* path, const float radiusArg) : radius(radiusArg)
{
	texture = new Texture(path, "", GL_TEXTURE_2D, ObjectType::DEFAULT);
	texture->LoadDDS();

	Compute();
	Store();
}

void Sphere::Compute()
{
	// ZONE  = triangle or square formed by intersection of 1 meridian strip and 1 parallel strip
	// THETA = ANGLE BETWEEN 2 ZONES OF ONE PARALLEL STRIP (in radians); 
	// PHI   = ANGLE BETWEEN 2 ZONES OF ONE MERIDIAN STRIP (in radians); 
	//-> using spherical coordinate system

	for (int i = 0; i <= nbParalStrips; ++i)
	{
		const float pi = glm::pi<float>();
		const float theta = pi * (0.5f - static_cast<float>(i) / static_cast<float>(nbParalStrips));
		const float rCosTheta = radius * glm::cos(theta);
		const float zCoor = radius * glm::sin(theta);

		for (int j = 0; j <= nbMeridStrips; ++j)
		{
			const float phi = 2.0f * pi * static_cast<float>(j) / static_cast<float>(nbMeridStrips);
			const float xCoor = rCosTheta * glm::cos(phi);
			const float yCoor = rCosTheta * glm::sin(phi);

			vertCoor.push_back(xCoor);
			vertCoor.push_back(yCoor);
			vertCoor.push_back(zCoor);

			const float invLength = 1.0f / radius;
			normalCoor.push_back(xCoor * invLength);
			normalCoor.push_back(yCoor * invLength);
			normalCoor.push_back(zCoor * invLength);

			textCoor.push_back(static_cast<float>(j) / nbMeridStrips);
			textCoor.push_back(static_cast<float>(i) / nbParalStrips);
		}
	}

	// indices
	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1
	for (int i = 0; i < nbParalStrips; ++i)
	{
		unsigned int index1 = i * (nbMeridStrips + 1);		// beginning of current parallel strip
		unsigned int index2 = index1 + nbMeridStrips + 1;	// beginning of next parallel strip

		for (int j = 0; j < nbMeridStrips; ++j, ++index1, ++index2)
		{
			// 2 triangles per zone except for those formed by first (top) and last (bottom) parallel strips
			if (i != 0)
			{
				indexes.push_back(index1);
				indexes.push_back(index2);
				indexes.push_back(index1 + 1);		// k1---k2---k1+1
			}

			if (i != (nbParalStrips - 1))
			{
				indexes.push_back(index1 + 1);
				indexes.push_back(index2);
				indexes.push_back(index2 + 1);		// k1+1---k2---k2+1
			}
		}
	}
}

void Sphere::Store()
{
	// Number of bytes encoding vertCoor / normalCoor / textCoor vectors
	const unsigned int sizeofVertices = sizeof(float) * vertCoor.size();
	const unsigned int sizeofNormals = sizeof(float) * normalCoor.size();
	const unsigned int sizeofTextures = sizeof(float) * textCoor.size();

	vao = new VertexArray();

	const std::vector<std::vector<float>*> dataAddresses{ &vertCoor, &normalCoor, &textCoor };
	VertexBuffer vbo(NULL, sizeofVertices + sizeofNormals + sizeofTextures);
	vbo.InitSubData(dataAddresses);

	const std::vector<unsigned int> sizeofs{ sizeofVertices, sizeofNormals, sizeofTextures };
	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vbl.Push<float>(2);
	vao->AddBuffer(vbo, vbl, sizeofs);

	ibo = new IndexBuffer(&indexes[0], indexes.size());

	// Configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	VertexArray lightVao;
	lightVao.Bind();
	vbo.Bind();

	// Note that we update the lamp's position attribute's stride to reflect the updated buffer data
	VertexBufferLayout lightVbl;
	lightVbl.Push<float>(3);
	lightVao.AddBuffer(vbo, lightVbl);

	ibo->Unbind();
	vbo.Unbind();
	vao->Unbind();
}

Sphere::~Sphere()
{
	//ibo->~IndexBuffer();
	//vao->~VertexArray();
}

void Sphere::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	texture->Enable(textureUnit);
	renderer.Draw(*vao, *ibo);
}



