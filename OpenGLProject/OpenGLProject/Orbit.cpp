
#include "Orbit.h"



Orbit::Orbit(const char * path, float radiusArg) : radius(radiusArg)
{
	texture = new Texture(path, "", GL_TEXTURE_2D, "default");
	texture->LoadDDS();

	Compute();
	Store();
}

void Orbit::Compute()
{
	float pi = glm::pi<float>(), theta;	
	nbMeridStrips = 100;				

	for (int i = 0; i <= nbMeridStrips; ++i)
	{
		theta = 2.0f * pi * (float)i / nbMeridStrips;

		vertCoor.push_back(radius * glm::sin(theta));
		vertCoor.push_back(0.0f);
		vertCoor.push_back(radius * glm::cos(theta));
	}
}

void Orbit::Store()
{
	unsigned int sizeofVertices = sizeof(float) * vertCoor.size();

	vao = new VertexArray();
	VertexBuffer vbo(&vertCoor[0], sizeofVertices);

	VertexBufferLayout vbl;
	vbl.Push<float>(3);			// Vertex coordinates (location = 0 in SphereShader.vs)
	vao->AddBuffer(vbo, vbl);

	vbo.Unbind();
	vao->Unbind();
}

Orbit::~Orbit()
{
	vao->~VertexArray();
}

void Orbit::Render()
{
	vao->Bind();
	glDrawArrays(GL_LINE_LOOP, 0, nbMeridStrips);
	vao->Unbind();
}

