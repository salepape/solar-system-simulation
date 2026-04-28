#include "SkyboxMeshComponent.h"

#include <glm/vec3.hpp>

#include <array>
#include <utility>
#include <vector>

#include "Rendering/Renderer.h"
#include "Utils/Constants.h"



SkyboxMeshComponent::SkyboxMeshComponent()
{
	ComputeVertices();
	StoreVertices();
};

void SkyboxMeshComponent::ComputeVertices()
{
	constexpr int32_t VERTEX_COORDINATE_COUNT = Vertex::POSITION_TYPE_DIMENSION * VERTEX_COUNT;

	// 6 faces of the cube defined
	std::array<float, static_cast<size_t>(VERTEX_COORDINATE_COUNT)> vertexCoor =
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
	vertices.reserve(static_cast<size_t>(VERTEX_COUNT));
	while (positionIndex < VERTEX_COORDINATE_COUNT)
	{
		Vertex vertex;
		vertex.position = glm::vec3(vertexCoor[positionIndex], vertexCoor[positionIndex + 1], vertexCoor[positionIndex + 2]);
		vertex.normal = GLMConstants::zeroVector;
		vertex.texCoords = GLMConstants::zeroVector;
		vertex.tangent = GLMConstants::zeroVector;
		vertex.biTangent = GLMConstants::zeroVector;
		vertices.push_back(std::move(vertex));

		positionIndex += Vertex::POSITION_TYPE_DIMENSION;
	}
}
