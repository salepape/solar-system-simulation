#include "Orbit.h"

#include <glad.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"



Orbit::Orbit(const std::string& texturePath, const float inRadius) :
	radius(inRadius),
	Mesh(texturePath, GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, TextureType::NONE)
{
	ComputeVertices();
	StoreVertices();
}

Orbit::~Orbit()
{

}

void Orbit::ComputeVertices()
{
	const float thetaAngle = 2.0f * glm::pi<float>() * 1.0f / meridianStripsCount;

	const glm::vec3 zeroVector(0.0f, 0.0f, 0.0f);

	vertices.reserve(meridianStripsCount + 1);
	for (int i = 0; i <= meridianStripsCount; ++i)
	{
		const float theta = static_cast<float>(i) * thetaAngle;

		Vertex vertex;
		vertex.Position = glm::vec3(radius * glm::sin(theta), 0.0f, radius * glm::cos(theta));
		vertex.Normal = zeroVector;
		vertex.TexCoords = zeroVector;
		vertex.Tangent = zeroVector;
		vertex.Bitangent = zeroVector;
		vertices.push_back(vertex);
	}
}

void Orbit::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	for (const auto& texture : textures)
	{
		texture.Enable(textureUnit);
	}
	
	renderer.Draw(*vao, GL_LINE_LOOP, meridianStripsCount);
}