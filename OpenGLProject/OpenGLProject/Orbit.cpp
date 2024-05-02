#include "Orbit.h"

#include <glad.h>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "Renderer.h"
#include "Texture.h"
#include "Utils.h"



Orbit::Orbit(const std::string& texturePath, const float inRadius) :
	radius(inRadius),
	Mesh(texturePath, GL_TEXTURE_2D, { GL_REPEAT }, { GL_LINEAR }, TextureType::NONE)
{
	ComputeVertices();
	StoreVertices();
}

void Orbit::ComputeVertices()
{
	const float thetaAngle = Utils::doublePi * 1.0f / meridianStripsCount;

	const glm::vec3 zeroVector(0.0f, 0.0f, 0.0f);

	vertices.reserve(meridianStripsCount + 1);
	for (uint32_t i = 0; i <= meridianStripsCount; ++i)
	{
		const float theta = i * thetaAngle;

		Vertex vertex;
		vertex.Position = glm::vec3(radius * glm::sin(theta), 0.0f, radius * glm::cos(theta));
		vertex.Normal = zeroVector;
		vertex.TexCoords = zeroVector;
		vertex.Tangent = zeroVector;
		vertex.Bitangent = zeroVector;
		vertices.push_back(vertex);
	}
}

void Orbit::Render(const Renderer& renderer, const uint32_t textureUnit) const
{
	for (const auto& texture : textures)
	{
		texture.Enable(textureUnit);
	}
	
	renderer.Draw(*vao, GL_LINE_LOOP, meridianStripsCount);

	for (const auto& texture : textures)
	{
		texture.Disable();
	}
}