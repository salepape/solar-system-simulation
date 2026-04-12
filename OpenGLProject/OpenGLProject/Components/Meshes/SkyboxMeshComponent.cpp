#include "SkyboxMeshComponent.h"

#include <array>
#include <glad.h>
#include <glm/vec3.hpp>
#include <utility>
#include <vector>

#include "OpenGLProject/Rendering/Renderer.h"
#include "OpenGLProject/Utils/Constants.h"



SkyboxMeshComponent::SkyboxMeshComponent()
{
	ComputeVertices();
	StoreVertices();
};

void SkyboxMeshComponent::ComputeVertices()
{
	std::array<float, 3 * VERTICES_COUNT> vertexCoor =
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	uint32_t positionIndex = 0;
	vertices.reserve(VERTICES_COUNT);
	while (positionIndex < VERTICES_COUNT * Vertex::POSITION_ELMTS_COUNT)
	{
		Vertex vertex;
		vertex.position = glm::vec3(vertexCoor[positionIndex], vertexCoor[positionIndex + 1], vertexCoor[positionIndex + 2]);
		vertex.normal = GLMConstants::zeroVector;
		vertex.texCoords = GLMConstants::zeroVector;
		vertex.tangent = GLMConstants::zeroVector;
		vertex.biTangent = GLMConstants::zeroVector;
		vertices.push_back(std::move(vertex));

		positionIndex += Vertex::POSITION_ELMTS_COUNT;
	}
}

void SkyboxMeshComponent::Render(const Renderer& renderer) const
{
	renderer.Draw(*vao, GL_TRIANGLES, VERTICES_COUNT);
}