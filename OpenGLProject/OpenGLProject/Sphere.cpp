#include "Sphere.h"



Sphere::Sphere(const char * path, float radiusArg) : radius(radiusArg)
{
	texture = new Texture(path, "", GL_TEXTURE_2D, "default");
	texture->LoadDDS();

	Compute();
	Store();
}

void Sphere::Compute()
{
	float invLength = 1.0f / radius;

	// ZONE  = triangle or square formed by intersection of 1 meridian strip and 1 parallel strip
	// THETA = ANGLE BETWEEN 2 ZONES OF ONE PARALLEL STRIP (in radians); 
	// PHI   = ANGLE BETWEEN 2 ZONES OF ONE MERIDIAN STRIP (in radians); 
	//-> using spherical coordinate system
	float pi = glm::pi<float>(), theta, rCosTheta, phi, xCoor, yCoor, zCoor;
	int nbParalStrips = 100, nbMeridStrips = 100;

	for (int i = 0; i <= nbParalStrips; ++i)
	{
		theta = pi * (0.5f - (float)i / (float)nbParalStrips);
		rCosTheta = radius * glm::cos(theta);
		zCoor = radius * glm::sin(theta);

		for (int j = 0; j <= nbMeridStrips; ++j)
		{
			phi = 2.0f * pi * (float)j / (float)nbMeridStrips;

			xCoor = rCosTheta * glm::cos(phi);
			yCoor = rCosTheta * glm::sin(phi);

			// Poisition of the current vertex 
			vertCoor.push_back(xCoor);
			vertCoor.push_back(yCoor);
			vertCoor.push_back(zCoor);

			// Outward normal of the current vertex 
			normalCoor.push_back(xCoor * invLength);
			normalCoor.push_back(yCoor * invLength);
			normalCoor.push_back(zCoor * invLength);

			// Texture coordinates of the current vertex
			textCoor.push_back((float)j / nbMeridStrips);
			textCoor.push_back((float)i / nbParalStrips);
		}
	}

	// indices
	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1
	unsigned int index1, index2;
	for (int i = 0; i < nbParalStrips; ++i)
	{
		index1 = i * (nbMeridStrips + 1);			// beginning of current parallel strip
		index2 = index1 + nbMeridStrips + 1;		// beginning of next parallel strip

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
	unsigned int  sizeofVertices = sizeof(float) * vertCoor.size();
	unsigned int  sizeofNormals = sizeof(float) * normalCoor.size();
	unsigned int  sizeofTextures = sizeof(float) * textCoor.size();

	std::vector<unsigned int> sizeofs{ sizeofVertices, sizeofNormals, sizeofTextures };
	std::vector<std::vector<float>*> dataAddresses { &vertCoor, &normalCoor, &textCoor };

	vao = new VertexArray();

	VertexBuffer vbo(NULL, sizeofVertices + sizeofNormals + sizeofTextures);
	vbo.InitSubData(dataAddresses);

	VertexBufferLayout vbl;
	vbl.Push<float>(3);						// Vertex position (location = 0 in SphereShader.vs)
	vbl.Push<float>(3);						// Vertex normal (location = 1 in SphereShader.vs)
	vbl.Push<float>(2);						// Vertex texture coordinates (location = 2 in SphereShader.vs)
	vao->AddBuffer2(vbo, vbl, sizeofs);

	ibo = new IndexBuffer(&indexes[0], indexes.size());

	// Configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	VertexArray lightVao;
	lightVao.Bind();
	vbo.Bind();
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
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

void Sphere::Render(Renderer& renderer, unsigned int& textureUnit)
{
	texture->Enable(textureUnit);
	renderer.Draw(*vao, *ibo);
}



