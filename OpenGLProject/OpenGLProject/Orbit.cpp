#include "Orbit.h"

#include <glad.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"



Orbit::Orbit(std::string texturePath, const float inRadius) : radius(inRadius)
{
	texture = new Texture(texturePath, GL_TEXTURE_2D, GeometryType::CIRCLE, MapType::NONE);
	texture->LoadDDS();
	textures.push_back(*texture);

	Compute();
	Store();
}

Orbit::~Orbit()
{
	delete texture;
}

void Orbit::Compute()
{
	for (int i = 0; i <= meridStripsCount; ++i)
	{
		const float theta = 2.0f * glm::pi<float>() * static_cast<float>(i) / meridStripsCount;

		Vertex vertex;
		vertex.Position = glm::vec3(radius * glm::sin(theta), 0.0f, radius * glm::cos(theta));
		vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
		vertex.TexCoords = glm::vec3(0.0f, 0.0f, 0.0f);
		vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
		vertex.Bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
		vertices.push_back(vertex);
	}
}

void Orbit::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	texture->Enable(textureUnit);
	renderer.Draw(*vao, GL_LINE_LOOP, meridStripsCount);
}