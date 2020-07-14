
#include "Orbit.h"



Orbit::Orbit(float radiusArg)
{
	radius = radiusArg;

	float pi = glm::pi<float>(), theta;
	nbMeridStrips = 100;
	std::vector<float> vertCoor;

	for (int i = 0; i <= nbMeridStrips; ++i)
	{
		theta = 2.0f * pi * (float)i / nbMeridStrips;

		vertCoor.push_back(radius * glm::sin(theta));
		vertCoor.push_back(0.0f);
		vertCoor.push_back(radius * glm::cos(theta));
	}



	int sizeofVertices = sizeof(float) * vertCoor.size();

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeofVertices, &vertCoor[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Orbit::Draw()
{
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINE_LOOP, 0, nbMeridStrips);
}

Orbit::~Orbit()
{
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAO);
}

