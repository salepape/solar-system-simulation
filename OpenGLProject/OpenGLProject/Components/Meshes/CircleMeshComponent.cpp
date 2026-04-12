#include "CircleMeshComponent.h"

#include <glad.h>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <utility>

#include "OpenGLProject/Rendering/Renderer.h"
#include "OpenGLProject/Utils/Constants.h"



CircleMeshComponent::CircleMeshComponent(const float inRadius) :
	radius(inRadius)
{
	ComputeVertices();
	StoreVertices();
}

void CircleMeshComponent::ComputeVertices()
{
	const float thetaAngle = GLMConstants::doublePi * 1.0f / meridianStripsCount;

	const glm::vec3 zeroVector(0.0f);

	vertices.reserve(meridianStripsCount + 1);
	for (uint32_t i = 0; i <= meridianStripsCount; ++i)
	{
		const float theta = i * thetaAngle;

		Vertex vertex;
		vertex.position = glm::vec3(radius * glm::sin(theta), 0.0f, radius * glm::cos(theta));
		vertex.normal = zeroVector;
		vertex.texCoords = zeroVector;
		vertex.tangent = zeroVector;
		vertex.biTangent = zeroVector;
		vertices.push_back(std::move(vertex));
	}
}

void CircleMeshComponent::Render(const Renderer& renderer) const
{
	renderer.Draw(*vao, GL_LINE_LOOP, meridianStripsCount);
}