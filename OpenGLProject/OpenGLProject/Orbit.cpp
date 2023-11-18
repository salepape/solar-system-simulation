#include "Orbit.h"



Orbit::Orbit(const char* path, const float radiusArg) : radius(radiusArg)
{
	texture = new Texture(path, "", GL_TEXTURE_2D, ObjectType::DEFAULT);
	texture->LoadDDS();

	Compute();
	Store();
}

void Orbit::Compute()
{
	nbMeridStrips = 100;				

	for (unsigned int i = 0; i <= nbMeridStrips; ++i)
	{
		const float theta = 2.0f * glm::pi<float>() * (float)i / nbMeridStrips;

		vertCoor.push_back(radius * glm::sin(theta));
		vertCoor.push_back(0.0f);
		vertCoor.push_back(radius * glm::cos(theta));
	}
}

void Orbit::Store()
{
	const unsigned int sizeofVertices = sizeof(float) * vertCoor.size();

	vao = new VertexArray();
	VertexBuffer vbo(&vertCoor[0], sizeofVertices);

	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vao->AddBuffer(vbo, vbl);

	vbo.Unbind();
	vao->Unbind();
}

Orbit::~Orbit()
{
	vao->~VertexArray();
}

void Orbit::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	texture->Enable(textureUnit);
	renderer.Draw(*vao, GL_LINE_LOOP, nbMeridStrips);
}

