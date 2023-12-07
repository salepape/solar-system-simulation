#include "Orbit.h"

#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Renderer.h"
#include "Texture.h"



Orbit::Orbit(std::string texturePath, const float radiusArg) : radius(radiusArg)
{
	texture = new Texture(texturePath, GL_TEXTURE_2D, GeometryType::CIRCLE, MapType::NONE);
	texture->LoadDDS();
	textures.push_back(*texture);

	Compute();
	Store();
}

Orbit::~Orbit()
{

}

void Orbit::Compute()
{
	for (int i = 0; i <= nbMeridStrips; ++i)
	{
		const float theta = 2.0f * glm::pi<float>() * static_cast<float>(i) / nbMeridStrips;

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
	renderer.Draw(*vao, GL_LINE_LOOP, nbMeridStrips);
}

