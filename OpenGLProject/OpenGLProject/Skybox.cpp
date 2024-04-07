#include "Skybox.h"

#include <glad.h>
#include <glm/vec3.hpp>
#include <vector>

#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"



Skybox::Skybox(const std::string& path)
{
	texture = new Texture(path, GL_TEXTURE_CUBE_MAP, GeometryType::CUBE, MapType::NONE);
	texture->LoadCubemapDDS();
	textures.push_back(*texture);

	Compute();
	Store();
};

void Skybox::Compute()
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
	
	int positionIndex = 0;
	vertices.reserve(VERTICES_COUNT);
	while (positionIndex < VERTICES_COUNT * Vertex::GetPositionElmtsCount())
	{
		Vertex vertex;
		vertex.Position = glm::vec3(vertCoor[positionIndex], vertCoor[positionIndex + 1], vertCoor[positionIndex + 2]);
		vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
		vertex.TexCoords = glm::vec3(0.0f, 0.0f, 0.0f);
		vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
		vertex.Bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
		vertices.push_back(vertex);

		positionIndex += Vertex::GetPositionElmtsCount();
	}
}

Skybox::~Skybox()
{
	delete texture;
}

void Skybox::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	if (texture)
	{
		texture->Enable(textureUnit);
		renderer.Draw(*vao, GL_TRIANGLES, 36);
	}
}