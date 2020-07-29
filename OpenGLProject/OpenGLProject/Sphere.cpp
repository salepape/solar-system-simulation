
#include "Sphere.h"



Sphere::Sphere(float radiusArg)
{
	radius = radiusArg;
	Compute();
	Linking();
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

void Sphere::Linking()
{
	// Number of bytes encoding vertCoor vector
	int sizeofVertices = sizeof(float) * vertCoor.size();
	int sizeofNormals = sizeof(float) * normalCoor.size();
	int sizeofTextures = sizeof(float) * textCoor.size();
	int sizeofIndexes = sizeof(unsigned int) * indexes.size();

	// GENERATION VBO - Generate 1 BO storing its ID / name in the provided array within graphics memory
	glGenBuffers(1, &VBO);
	// BINDING VBO - Bind BO to the OpenGL BO target we want (here the one of a vertex, called GL_ARRAY_BUFFER, hence VBO variable name)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// COPY - Allocate memory and store data into the currently bound BO memory
	glBufferData(GL_ARRAY_BUFFER, sizeofVertices + sizeofNormals + sizeofTextures, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeofVertices, &vertCoor[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeofVertices, sizeofNormals, &normalCoor[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeofVertices + sizeofNormals, sizeofTextures, &textCoor[0]);

	// GENERATION EBO - Generate 1 BO for index elements
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofIndexes, &indexes[0], GL_STATIC_DRAW);

	// GENERATION VAO - Generate 1 VAO 
	glGenVertexArrays(1, &VAO);
	// BINDING VAO - Bind VAO, which thus will store all following VBO calls
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// PERMISSION - Enable the vertex attribute provided its location
	glEnableVertexAttribArray(0);
	// INTERPRETATION - Store in the currently bound VBO how OpenGL should interpret the vertex buffer data			   -> POSITION 0 FOR VERT COOR
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeofVertices));					// -> POSITION 1 FOR NORMAL COOR
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeofVertices + sizeofNormals));	// -> POSITION 2 FOR TEXT COOR

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// UNBIND - Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Sphere::Draw()
{
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, indexes.size() , GL_UNSIGNED_INT, (void*)0);
}

Sphere::~Sphere()
{
	// CLEAN EBO (optional) - Free the resource once it've outlived its purpose
	glDeleteBuffers(1, &EBO);

	// CLEAN VBO (optional) - Free the resource once it've outlived its purpose
	glDeleteBuffers(1, &VBO);

	// CLEAN VAO (optional) - Free the resource once it've outlived its purpose
	glDeleteVertexArrays(1, &VAO);
}

