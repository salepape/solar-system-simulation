#include "Skybox.h"

#include <glad.h>
#include <glm/vec3.hpp>
#include <vector>

#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"



Skybox::Skybox(const std::string& texturePath)
{
	Texture texture(texturePath, GL_TEXTURE_CUBE_MAP, GL_CLAMP_TO_EDGE, GL_LINEAR, TextureType::NONE);
	texture.LoadCubemapDDS();
	textures.push_back(texture);

	ComputeVertices();
	StoreVertices();
};

void Skybox::ComputeVertices()
{
	// FacesCount is 6, verticesPerFaceCount is 6
	constexpr int VERTICES_COUNT = 36;

	std::vector<float> vertCoor =
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

	const glm::vec3 zeroVector(0.0f, 0.0f, 0.0f);
	
	int positionIndex = 0;
	vertices.reserve(VERTICES_COUNT);
	while (positionIndex < VERTICES_COUNT * Vertex::POSITION_ELMTS_COUNT)
	{
		Vertex vertex;
		vertex.Position = glm::vec3(vertCoor[positionIndex], vertCoor[positionIndex + 1], vertCoor[positionIndex + 2]);
		vertex.Normal = zeroVector;
		vertex.TexCoords = zeroVector;
		vertex.Tangent = zeroVector;
		vertex.Bitangent = zeroVector;
		vertices.push_back(vertex);

		positionIndex += Vertex::POSITION_ELMTS_COUNT;
	}
}

Skybox::~Skybox()
{

}

void Skybox::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	for (const auto& texture : textures)
	{
		texture.Enable(textureUnit);
	}
	
	renderer.Draw(*vao, GL_TRIANGLES, 36);
}