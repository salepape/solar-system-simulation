#include "CircleMeshComponent.h"

#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include <utility>

#include "Rendering/Renderer.h"
#include "Utils/Constants.h"



CircleMeshComponent::CircleMeshComponent(const float inRadius, const uint32_t inMeridianStripCount) :
	radius(inRadius), meridianStripCount(inMeridianStripCount)
{
	ComputeVertices();
	StoreVertices();
}

void CircleMeshComponent::ComputeVertices()
{
	const float thetaAngle = GLMConstants::doublePi * 1.0f / meridianStripCount;

	const glm::vec3 zeroVector(0.0f);

	vertices.reserve(static_cast<size_t>(meridianStripCount + 1));
	for (int32_t i = 0; i <= meridianStripCount; ++i)
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

void CircleMeshComponent::Render(const unsigned int /*mode*/) const
{
	MeshComponent::Render(GL_LINE_LOOP);
}